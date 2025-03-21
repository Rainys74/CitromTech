#include "Renderer.h"
#include "ArgumentHandler.h"
#include "Application/ApplicationInfo.h"

#include "Renderer/RenderAPI/Buffer.h"
#include "Vendor/stb/stb_image_write.h"
#include "Vendor/stb/stb_image.h"
#include "Vendor/tinyobjloader/tiny_obj_loader.h"

#include "EntitySystem/Components/EssentialComponents.h"
#include "EntitySystem/Components/RendererComponents.h"
#include "EntitySystem/Entity.h"

#include "ShaderInterop/Matrices_ShaderInterop.h"
#include "ShaderInterop/Lighting_ShaderInterop.h"
#include "ShaderInterop/StandardMaterial_ShaderInterop.h"

#include "Math/Vector.h"

#include <filesystem>
#include <fstream> // Renderer_SaveMaterialsToFiles

#define INCREMENT_DRAW_CALL()

namespace Citrom
{
	using namespace RenderAPI;

	RenderAPI::Device* Renderer::m_Device;
	EventListener<WindowEvents> Renderer::s_WindowEventListener;

	Renderer::CameraData Renderer::s_CurrentCamera;
	Scene* Renderer::s_CurrentScene;

	static EditorRenderer g_EditorRenderer;

	static CTL::HashMap<std::string, Shader, CTL::StdStringHash, CTL::StdStringHashEqual> g_Shaders;
	static CTL::StdStrHashMap<Material*> g_Materials; // no need for mat instances since materials here are just shader wrappers with uniforms built-in

	CTL::StdStrHashMap<Shader>& Renderer_GetShaders()
	{
		return g_Shaders;
	}

	void Renderer_RecompileShader(const std::string& shaderName)
	{
		ShaderDesc sd;
		sd.name = shaderName;
		g_Shaders[shaderName].internal.reset();
		g_Shaders[shaderName] = DEVICE->CreateShader(&sd);
	}
	void Renderer_RecompileAllShaders()
	{
		for (const auto& shaderPair : g_Shaders)
			Renderer_RecompileShader(shaderPair.first);
	}
	static std::string Renderer_GetShaderName(const RenderAPI::Shader& shader)
	{
		for (const auto& shaderPair : g_Shaders)
		{
			//if (shaderPair.second == shader)
			//	return shaderPair.first;
			//if (shaderPair.second.descriptor.name == shader.descriptor.name)
		}
		return "Unknown";
	}

	CTL::StdStrHashMap<Material*>& Renderer_GetMaterials()
	{
		return g_Materials;
	}

	Material* Renderer_GetMaterial(const std::string& name)
	{
		return g_Materials[name];
	}
	Material* Renderer_CreateMaterial(const std::string& materialName, const std::string& shaderName)
	{
		if (g_Materials.count(materialName) > 0)
		{
			delete g_Materials[materialName];
		}

		return g_Materials[materialName] = new Material(*Renderer_GetShader(shaderName), &materialName);
	}
	Material* Renderer_GetOrCreateMaterial(const std::string& materialName, const std::string& shaderName)
	{
		// If the material exists, return it
		if (g_Materials.count(materialName) > 0)
		{
			return g_Materials[materialName];
		}

		return Renderer_CreateMaterial(materialName, shaderName);
	}
	Material* Renderer_CreateMaterialFromData(const MaterialData& matData)
	{
		if (g_Materials.count(matData.materialName) > 0)
		{
			delete g_Materials[matData.materialName];
		}

		return g_Materials[matData.materialName] = new Material(matData);
	}

	bool Renderer_HasMaterial(const std::string& materialName)
	{
		if (g_Materials.count(materialName) > 0)
			return true;
		return false;
	}

	void Renderer_SaveMaterialsToFiles()
	{
		for (const auto& matPair : g_Materials)
		{
			const Material& mat = *matPair.second;

			std::string source = JSON::SerializeObject(mat, JSON::SerializerOptions(true));

			std::string filePath("Assets/Materials/");
			filePath.append(mat.GetName());
			filePath.append(".ctmat");
			
			std::ofstream outFile(filePath);
			CT_CORE_ASSERT(outFile.is_open(), "Unable to open material file!");
			outFile << source;
			outFile.close();
		}
	}

	struct RendererData
	{
		ShaderInterop::Lighting lighting;

		bool test;
		struct CameraDataTemp // TODO: is this superior to the current setup?
		{
			bool test;
		} cameraData;
	};
	static RendererData g_RendererData;

	static void ResetRendererData()
	{
		//Memory::Zero(&g_RendererData, sizeof(g_RendererData));
		g_RendererData = RendererData();
	}

	void Renderer::Initialize(Platform::Window* window)
	{
		//GraphicsAPIManager::ForceGraphicsAPI(GraphicsAPI::DirectX11);
		//GraphicsAPIManager::ForceGraphicsAPI(GraphicsAPI::OpenGL);
		//GraphicsAPIManager::PrioritizeGraphicsAPI(GraphicsAPI::DirectX11);
        //GraphicsAPIManager::ForceGraphicsAPI(GraphicsAPI::Metal);

		// TODO: load some files to prioritize api's or something, or well just keep it this way.
		for (size_t i = 0; i < CT_ARRAY_LENGTH(MainApplicationSpec.rendererInfo.apiPriorityList); i++)
			GraphicsAPIManager::PrioritizeGraphicsAPI(MainApplicationSpec.rendererInfo.apiPriorityList[i], i);

		if (ArgumentHandler::HasArgument("-force-d3d11"))
			GraphicsAPIManager::ForceGraphicsAPI(GraphicsAPI::DirectX11);
        else if (ArgumentHandler::HasArgument("-force-metal"))
            GraphicsAPIManager::ForceGraphicsAPI(GraphicsAPI::Metal);
		
		CT_TRACE("CURRENT API: {} ({})", GraphicsAPIManager::ToString(GraphicsAPIManager::GetGraphicsAPI()), (size_t)GraphicsAPIManager::GetGraphicsAPI());
		for (size_t i = 0; i < (size_t)GraphicsAPI::Count; i++)
		{
			CT_TRACE("{}. {} ({})", i, GraphicsAPIManager::ToString(GraphicsAPIManager::GetGraphicsAPIAtPriority(i)), (size_t)GraphicsAPIManager::GetGraphicsAPIAtPriority(i));
		}

		//m_Device = CTL::CreateScopedPtr<RenderAPI::Device>();
		m_Device = Device::Get();

		SwapChainDesc scd;
		scd.windowPtr = window;
		scd.renderFormat = Format::R8G8B8A8_U2FNORM;

		BlendStateDesc bsd;
		bsd.srcBlend = BlendFactor::SrcAlpha;
		bsd.destBlend = BlendFactor::OneMinusSrcAlpha;
		bsd.blendOperation = BlendOp::Add;

		bsd.renderTargetWriteMask = RenderTargetWriteMask::All;

		RasterizerStateDesc rsd;
		rsd.fillMode = FillMode::Solid;
		rsd.cullMode = CullMode::Back;
		rsd.frontCounterClockwise = false;

		DepthStencilStateDesc dsd;
		dsd.depthEnabled = true;
		dsd.depthWriteEnabled = true;
		dsd.depthFunc = DepthStencilComparisonFunc::Less;

		PipelineStateDesc psd;
		psd.blendState = &bsd;
		psd.rasterizerState = &rsd;
		psd.dsState = &dsd;
		psd.primitiveType = PrimitiveTopology::Triangles;

		m_Device->SetVSync(VSyncMode::On);
		m_Device->MakeSwapChain(&scd);

		// TODO: you're gonna have to remake the entire renderer to use shaders with pipelines lol
		//PipelineState pipeline = m_Device->CreatePipelineState(&psd);
		//m_Device->BindPipelineState(&pipeline);

		// On Resize Callback
		s_WindowEventListener.OnEvent = [](const Event<WindowEvents>& event)
		{
			if (event.GetEventType() == WindowEvents::WindowResize)
			{
				const WindowResizeEvent& transformedEvent = (const WindowResizeEvent&)event;
				Device::Get()->Resize(transformedEvent.width, transformedEvent.height);
				// TODO: Resize Framebuffers
			}
		};
		EventBus::GetDispatcher<WindowEvents>()->AddListener(&s_WindowEventListener);

		GPUInfo gpuInfo = m_Device->GetCurrentGPUInfo();
		CT_CORE_ERROR("{} Adapter Info:", GraphicsAPIManager::ToString(GraphicsAPIManager::GetGraphicsAPI()));
        CT_CORE_ERROR("\tVendor: {}", gpuInfo.vendor);
        CT_CORE_ERROR("\tRenderer: {}", gpuInfo.renderer);
        CT_CORE_ERROR("\tVersion: {}", gpuInfo.version);
        CT_CORE_ERROR("\tShading Language Version: {}", gpuInfo.shadingLanguageVersion);
        CT_CORE_ERROR("\tTotal VRAM: {} MB", gpuInfo.videoMemory / (1 << 20)); // 1024 * 1024, 1 << 20, 1e+6 (10 ^ +6 = 1'000'000)

		/*constexpr const char* shaderPath = "Shaders/";
		constexpr const char* shaderCachePath = "ShaderCache/";

		const std::string writtenShaderPaths[] =
		{
			shaderPath,
			"Assets/Shaders/"
		};*/

		const std::string shaderPaths[] =
		{
			"Shaders/",
			"Assets/Shaders/",

			"ShaderCache/"
		};

		//ShaderCompiler::ClearShaderCache("ShaderCache/");

		// TODO: 2 might need to be changed to 3, as well as shaders might need
		// to be transpiled after compilation if i plan on implementing HLSLcc
		ShaderCompiler::PrepareShaders(shaderPaths, 2, "ShaderCache/");
		//ShaderCompiler::CompileShaders(shaderPaths, 3, "ShaderCache/");

		//CTL::HashMap<std::string, Shader, CTL::StdStringHash, CTL::StdStringHashEqual> uniqueShaderNames;
		//for (const auto& entry : std::filesystem::directory_iterator("ShaderCache/"))
		//{
		//	const std::string fileName = entry.path().filename().string();
		//
		//	// Find the first underscore and extract the base name
		//	size_t pos = fileName.find('_');
		//	if (pos != std::string::npos) 
		//	{
		//		if (fileName.substr(pos + 1, 2) == "cs")
		//			continue;
		//		std::string baseName = fileName.substr(0, pos);
		//
		//		ShaderDesc sd;
		//		sd.name = baseName;
		//
		//		uniqueShaderNames[baseName] = m_Device->CreateShader(&sd);
		//	}
		//}
		//for (const auto& shaderPair : uniqueShaderNames)
		//{
		//	g_Shaders.PushBack(shaderPair.second);
		//}

		// Shaders
		for (const auto& entry : std::filesystem::directory_iterator("ShaderCache/"))
		{
			const std::string fileName = entry.path().filename().string();

			// Find the first underscore and extract the base name
			const size_t pos = fileName.find('_');
			if (pos != std::string::npos)
			{
				if (fileName.substr(pos + 1, 2) == "cs")
					continue;
				std::string baseName = fileName.substr(0, pos);

				ShaderDesc sd;
				sd.name = baseName;

				g_Shaders[baseName] = m_Device->CreateShader(&sd);
			}
		}

		// Materials
		for (const auto& entry : std::filesystem::directory_iterator("Assets/Materials/"))
		{
			if (entry.path().filename().extension() != ".ctmat") // .ctmat, .cmtl, .cmt, .ctmt, .ctmtl
				continue;

			const std::string fileName = entry.path().filename().string();
			const std::string fileNameNoExt = entry.path().filename().stem().string();

			std::ifstream file(entry.path());
			CT_CORE_ASSERT(file.is_open(), "Failed to open material file for writing!");

			std::string jsonSrc((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

			MaterialData matData = JSON::DeserializeObject<MaterialData>(jsonSrc);
			matData.materialName = fileNameNoExt;

			// TODO: also add support for recreating/creating materials on the fly (runtime) for easier editor work.
			Renderer_CreateMaterialFromData(matData);
		}

		//ShaderInterop::SetupStandardMaterial(ShaderInterop::StandardMaterial(), Renderer_GetOrCreateMaterial("Standard", "Standard"));
		const std::string standardMatName = "Standard";
		if (!Renderer_HasMaterial(standardMatName)) // we are making an assumption that if the renderer has the standard material, it means it was loaded correctly.
			ShaderInterop::SetupStandardMaterial(ShaderInterop::StandardMaterial(), Renderer_GetOrCreateMaterial("Standard", "Standard"));

		g_EditorRenderer.Initialize();
	}

	void Renderer::BeginFrame(Scene* scene, Camera* camera, Math::Transform* cameraTransform)
	{
		s_CurrentCamera = CameraData(camera, cameraTransform);
		s_CurrentScene = scene;
		// TODO: set up lighting data

		/*if (s_CurrentScene && s_CurrentCamera.camera)
		{
			Math::Color& clearColor = s_CurrentCamera.camera->clearColor;
			m_Device->RCClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		}
		else
		{
			//m_Device->RCClearColor(1.0f, 0.5f, 0.24f, 1.0f); // Citrom Tech 2D
			m_Device->RCClearColor(0.5f, 0.74f, 0.14f); // Citrom Tech
		}*/

		/*RenderPassDesc rpd = {};
		rpd.targetFramebuffer = nullptr;

		if (s_CurrentCamera.camera)
			rpd.clearColor = { s_CurrentCamera.camera->clearColor.r, s_CurrentCamera.camera->clearColor.g, s_CurrentCamera.camera->clearColor.b, s_CurrentCamera.camera->clearColor.a};
		else
			rpd.clearColor = { 0.5f, 0.74f, 0.14f, 0.0f };

		RenderPass pass = m_Device->CreateRenderPass(&rpd);

		m_Device->RCBegin();
		m_Device->RCBeginRenderPass(&pass);*/
        // Render passes are NOT the job for begin/end frame!
        
        m_Device->RCBegin();
	}

	void Renderer::EndFrame()
	{
		//m_Device->RCEndRenderPass();
		m_Device->RCEnd();

		//m_Device->WaitForGPU();

		m_Device->SwapBuffers();

		s_CurrentScene = nullptr;
	}

	//void Renderer::Begin(Scene* scene)
	//{
	//	s_CurrentScene = scene;
	//	s_CurrentCamera.camera = &scene->GetMainCameraEntity().GetComponent<CameraComponent>().camera;
	//	s_CurrentCamera.cameraTransform = &scene->GetMainCameraEntity().GetComponent<TransformComponent>().transform;
	//}
	//
	//void Renderer::End()
	//{
	//}

	void Renderer::SubmitScene(Scene* scene)
	{
		// TODO: make sure it uses submittion
		// Right now it draws immediately
		auto view = scene->GetAllEntitiesWith<CubeComponent>();
		for (auto cube : view)
		{
			auto& transformComponent = Entity(cube, scene).GetComponent<TransformComponent>();
			auto& cubeComponent = view.get<CubeComponent>(cube);

			static constexpr float32 positions[] =
			{
				// Front face
				-0.5f, -0.5f,  0.5f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f, // Bottom-left
				 0.5f, -0.5f,  0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, // Bottom-right
				 0.5f,  0.5f,  0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // Top-right
				-0.5f,  0.5f,  0.5f,		0.0f, 0.0f, 1.0f,		0.0f, 1.0f, // Top-left

				// Back face
				-0.5f, -0.5f, -0.5f,		0.0f, 0.0f, -1.0f,		0.0f, 0.0f, // Bottom-left
				 0.5f, -0.5f, -0.5f,		0.0f, 0.0f, -1.0f,		1.0f, 0.0f, // Bottom-right
				 0.5f,  0.5f, -0.5f,		0.0f, 0.0f, -1.0f,		1.0f, 1.0f, // Top-right
				-0.5f,  0.5f, -0.5f,		0.0f, 0.0f, -1.0f,		0.0f, 1.0f, // Top-left
			};
			static constexpr uint32 indices[] =
			{
				// Front face
				0, 1, 2,
				2, 3, 0,

				// Back face
				4, 5, 6,
				6, 7, 4,

				// Left face
				4, 0, 3,
				3, 7, 4,

				// Right face
				1, 5, 6,
				6, 2, 1,

				// Top face
				3, 2, 6,
				6, 7, 3,

				// Bottom face
				4, 5, 1,
				1, 0, 4,
			};

			Mesh cubeMesh;

			for (size_t i = 0; i < CT_ARRAY_LENGTH(positions) / 8; i++)
			{
				Vertex v;
				v.position = Math::Vector3(positions[i * 8 + 0], positions[i * 8 + 1], positions[i * 8 + 2]);
				v.normal = Math::Vector3(positions[i * 8 + 3], positions[i * 8 + 4], positions[i * 8 + 5]);
				v.texCoord = { positions[i * 8 + 6], positions[i * 8 + 7] };

				cubeMesh.vertices.PushBack(v);
			}

			for (size_t i = 0; i < CT_ARRAY_LENGTH(indices); i++)
				cubeMesh.indices.PushBack(indices[i]);

			struct ConstantBufferTest
			{
				Math::Matrix4x4 transform;
			};
			ConstantBufferTest cbt = {};

			Math::Matrix4x4 projection = s_CurrentCamera.camera->GetProjection();
			Math::Matrix4x4 view = s_CurrentCamera.cameraTransform->GetCameraViewFromTransform();
			Math::Matrix4x4 model = transformComponent.transform.GetTransformMatrix();
			cbt.transform = projection * view * model;
			cbt.transform.Transpose();

			UniformBufferDesc ubd = {};
			ubd.data = &cbt;
			ubd.dataBytes = sizeof(cbt);
			ubd.usage = Usage::Dynamic;

			UniformBuffer ub = m_Device->CreateUniformBuffer(&ubd);
			m_Device->RCBindUniformBuffer(&ub);

			ShaderDesc sd = { "Standard" };
			Shader shader = m_Device->CreateShader(&sd);

			Material defaultMaterialTest(shader);
			RenderMeshWithMaterial(cubeMesh, defaultMaterialTest);
		}
	}

	void Renderer::DrawTest(Camera* camera, Math::Transform* cameraTransform)
	{
		CT_PROFILE_STATIC_FUNCTION(Renderer);

		//Camera* camera = s_CurrentCamera.camera;
		//Math::Transform* cameraTransform = s_CurrentCamera.cameraTransform;

        RenderPassDesc rpd = {};
        rpd.targetFramebuffer = nullptr;

        if (s_CurrentCamera.camera)
            rpd.clearColor = { s_CurrentCamera.camera->clearColor.r, s_CurrentCamera.camera->clearColor.g, s_CurrentCamera.camera->clearColor.b, s_CurrentCamera.camera->clearColor.a};
        else
            rpd.clearColor = { 0.5f, 0.74f, 0.14f, 0.0f };

        RenderPass pass = m_Device->CreateRenderPass(&rpd);

        m_Device->RCBeginRenderPass(&pass);
		m_Device->RCSetViewport(ViewportSpecification(camera->viewportSize.x, camera->viewportSize.y, camera->viewportOffset.x, camera->viewportOffset.y));
        
		g_EditorRenderer.Render(camera, cameraTransform);
        //m_Device->RCEndRenderPass(); // End render pass before returning!
        //return;

		//static CTL::DArray<float> vertices;
		static CTL::DArray<Vertex> vertexes;
		static CTL::DArray<unsigned int> indices;
		static std::string textureName = "texture.png";

		static Texture2D tex2D;

		float triangleVertices[] =
		{
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
			 0.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		0.5f, 1.0f,
			 0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f
		};

		unsigned int triangleIndices[] =
		{
			0, 1, 2//,
			//2, 3, 0
		};
		//for (size_t i = 0; i < CT_ARRAY_LENGTH(triangleVertices); i++)
		//	vertices.PushBack(triangleVertices[i]);
		//for (size_t i = 0; i < CT_ARRAY_LENGTH(triangleIndices); i++)
		//	indices.PushBack(triangleIndices[i]);

		if (vertexes.Count() == 0 && indices.Count() == 0)
		{
			tinyobj::attrib_t attrib;
			std::vector<tinyobj::shape_t> shapes;
			std::vector<tinyobj::material_t> materials;
			std::string warn, err;

			CT_CORE_VERIFY(tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "dinosaur.obj"), "TinyObj failed to load!");

			for (size_t s = 0; s < shapes.size(); s++)
			{
				CT_WARN("Shape[{}].name = {}", s, shapes[s].name);
				CT_WARN("Shape[{}].indices = {}", s, shapes[s].mesh.indices.size());
			}

			/*vertexes.Reserve(attrib.vertices.size() / 3);
			for (size_t v = 0; v < attrib.vertices.size() / 3; v++)
				vertexes.PushBack(Vertex{});

			for (size_t v = 0; v < attrib.vertices.size() / 3; v++)
			{
				vertexes[v].position.x = attrib.vertices[3 * v + 0];
				vertexes[v].position.y = attrib.vertices[3 * v + 1];
				vertexes[v].position.z = attrib.vertices[3 * v + 2];
			}*/

			/*for (size_t v = 0; v < attrib.texcoords.size() / 2; v++)
			{
				vertexes[v].texCoord.u = attrib.texcoords[2 * v + 0];
				vertexes[v].texCoord.v = attrib.texcoords[2 * v + 1];
			}

			for (const auto& shape : shapes)
			{
				for (const auto& index : shape.mesh.indices)
				{
					indices.PushBack(index.vertex_index);
				}
			}*/
			for (const auto& shape : shapes)
			{
				for (const auto& index : shape.mesh.indices)
				{
					Vertex vert = {}; // Create a new vertex

					// Get the correct position
					vert.position.x = attrib.vertices[3 * index.vertex_index + 0];
					vert.position.y = attrib.vertices[3 * index.vertex_index + 1];
					vert.position.z = attrib.vertices[3 * index.vertex_index + 2];

					// Get the correct normal, ensuring the normal index is valid
					if (index.normal_index >= 0)
					{
						vert.normal.x = attrib.normals[3 * index.normal_index + 0];
						vert.normal.y = attrib.normals[3 * index.normal_index + 1];
						vert.normal.z = attrib.normals[3 * index.normal_index + 2];
					}

					// Get the correct texture coordinate, ensuring the texcoord index is valid
					if (index.texcoord_index >= 0)
					{
						vert.texCoord.u = attrib.texcoords[2 * index.texcoord_index + 0];
						vert.texCoord.v = 1.0f - attrib.texcoords[2 * index.texcoord_index + 1]; // Flip V-axis
					}

					vertexes.PushBack(vert);
					indices.PushBack(vertexes.Count() - 1); // Store the new vertex index
				}
			}
			
			for (const auto& material : materials)
			{
				CT_WARN("Material name: {}", material.name);
				if (!material.diffuse_texname.empty())
				{
					CT_WARN("Diffuse texture: {}", material.diffuse_texname);
					textureName = material.diffuse_texname;
				}
			}
		}

		CTL::DArray<float> vertices(vertexes.Size());
		//vertices.Reserve((sizeof(Vertex) / 4) * vertexes.Count());
		for (const Vertex& vert : vertexes)
		{
			vertices.PushBack(vert.position.x);
			vertices.PushBack(vert.position.y);
			vertices.PushBack(vert.position.z);

			vertices.PushBack(vert.normal.x);
			vertices.PushBack(vert.normal.y);
			vertices.PushBack(vert.normal.z);

			vertices.PushBack(vert.texCoord.u);
			vertices.PushBack(vert.texCoord.v);
		}

		// Index Buffer
		IndexBufferDesc ibd = {};
		ibd.data = indices.Data();
		ibd.count = indices.Count();

		IndexBuffer ibo = m_Device->CreateIndexBuffer(&ibd);
		m_Device->RCBindIndexBuffer(&ibo);
		Device_SetDebugName(&ibo, "Renderer::DrawTest() Main Index Buffer (ibo)");

		// Shader
		ShaderDesc sd = {};
		sd.name = "Standard";

		/*static */Shader shader = m_Device->CreateShader(&sd);
		//m_Device->BindShader(&shader); // in pipeline

		// Shader Constant Buffer
		/*struct alignas(16) ConstantBufferTest
		{
			Math::Matrix4x4 transform;
			//Math::Vector3 directionalLightDir;
			//uint8 padding1[sizeof(float)];
			Math::Vector3 cameraLocalPos;
		};
		ConstantBufferTest cbt = {};*/
		ShaderInterop::Matrices cbt = {};
		Math::Matrix4x4 projection;
		//projection.Orthographic(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		//projection.Orthographic(-2.67f, 2.67f, -1.5f, 1.5f, -1.0f, 1.0f);
		projection.Perspective(Math::DegreesToRadians(90.0f), 16.0f / 9.0f, 0.01f, 1000.0f); // also Z positive and negative values would be great to reverse them (try glm::inverse counterpart?)
		projection = camera->GetProjection();
		Math::Matrix4x4 view = Math::Matrix4x4::Translate(Math::Matrix4x4::Identity(), -cameraTransform->position);
		Math::Matrix4x4 translationView = Math::Matrix4x4::Translate(Math::Matrix4x4::Identity(), -cameraTransform->position);
		Math::Matrix4x4 rotationView = Math::Matrix4x4::FromQuaternion(cameraTransform->rotation);
		//view = rotationView * translationView; // The most correct right now.
		//view = Math::Matrix4x4::Inverse(cameraTransform->GetTransformMatrix());
		view = cameraTransform->GetCameraViewFromTransform();
		/*Math::Matrix4x4 model = Math::Matrix4x4::FromQuaternion(Math::Quaternion::Euler(0.0f, Math::DegreesToRadians(45.0f), 0.0f)) * Math::Matrix4x4::Translate(Math::Matrix4x4::Identity(), Math::Vector3(1.0f, 2.0f, 3.0f)); */
		Math::Matrix4x4 model = Math::Matrix4x4::Translate(Math::Matrix4x4::Identity(), Math::Vector3(0.0f, 0.0f, 0.0f));
		cbt.transform = projection * view * model;
		//cbt.transform = model * view * projection; // INCORRECT!

		ResetRendererData();
		if (s_CurrentScene)
		{
			// Directional Light
			{
				auto view = s_CurrentScene->GetAllEntitiesWith<DirectionalLightComponent>();
				for (auto dirLight : view)
				{
					auto& dirLightComponent = Entity(dirLight, s_CurrentScene).GetComponent<DirectionalLightComponent>();
					auto& transformComponent = Entity(dirLight, s_CurrentScene).GetComponent<TransformComponent>();

					g_RendererData.lighting.directionalLights[0].direction = transformComponent.transform.Forward();
					g_RendererData.lighting.directionalLights[0].base.color = Math::ColorF32x4(dirLightComponent.color, dirLightComponent.intensity);

					g_RendererData.lighting.directionalLightCount++;
				}
			}
			// Sky Light
			{
				auto view = s_CurrentScene->GetAllEntitiesWith<SkyLightComponent>();
				for (auto skyLight : view)
				{
					auto& skyLightComponent = Entity(skyLight, s_CurrentScene).GetComponent<SkyLightComponent>();

					g_RendererData.lighting.skyLight.base.color = Math::ColorF32x4(skyLightComponent.color, skyLightComponent.intensity);
				}
			}
			/*for (size_t i = 0; i < view.size(); i++)
			{
				//auto& dirLight = view.get(view[i]);
				//auto& dirLight = view.get<DirectionalLightComponent>(view.storage<3>());
				//auto& dirLight = view.get<i>();
			}*/
		}
		Math::Vector3 lightDirectionWorldSpace = g_RendererData.lighting.directionalLights[0].direction; //Math::Vector3(0.0f, -1.0f, 0.0f); // probably use -transform.forward for getting the light direction
		Math::Vector3 lightDirectionLocalSpace = (Math::Matrix4x4::Inverse(model) * lightDirectionWorldSpace).Normalized();
		//CT_CORE_VERBOSE("WORLD SPACE: {}", lightDirectionWorldSpace.ToString());
		//CT_CORE_VERBOSE("LOCAL SPACE: {}", lightDirectionLocalSpace.ToString());

		/*
		// Step 1: Extract Camera Position in World Space
		Math::Matrix4x4 invViewMatrix = Math::Matrix4x4::Inverse(view);
		Math::Vector3 cameraPositionWorldSpace = Math::Vector3(invViewMatrix[3][0], invViewMatrix[3][1], invViewMatrix[3][2]);

		// Step 2: Transform Camera Position to Local Space
		Math::Matrix4x4 invModelMatrix = Math::Matrix4x4::Inverse(model);
		Math::Vector3 cameraPositionLocalSpace = invModelMatrix * cameraPositionWorldSpace;
		*/
		Math::Vector3 cameraPositionLocalSpace = Math::Matrix4x4::Inverse(model) * cameraTransform->position;
		//CT_CORE_VERBOSE("CAMERA POS WORLD SPACE: {}", cameraTransform->position.ToString());
		//CT_CORE_VERBOSE("CAMERA POS LOCAL SPACE: {}", cameraPositionLocalSpace.ToString());

		cbt.transform.Transpose();
		//cbt.directionalLightDir = lightDirectionLocalSpace;
		cbt.cameraLocalPos = cameraPositionLocalSpace;

		{
			UniformBufferDesc ubld = {};
			ubld.data = &g_RendererData.lighting;
			ubld.dataBytes = sizeof(g_RendererData.lighting);
			ubld.usage = Usage::Dynamic;

			UniformBuffer ubl = m_Device->CreateUniformBuffer(&ubld);
			//m_Device->RCBindUniformBuffer(&ubl, ShaderType::Vertex, CTSI_CBSLOT_LIGHTING); //TODO: Temp Metal
			m_Device->RCBindUniformBuffer(&ubl, ShaderType::Fragment, CTSI_CBSLOT_LIGHTING); //TODO: Temp Metal // for lighting!

			m_Device->SetUniformBufferData(&ubl, ubld.data, ubld.dataBytes);
		}

		static ShaderInterop::StandardMaterial standardMat;
		standardMat.mat_Albedo = CTSI_TYPE_COLOR3(1.0f, 1.0f, 1.0f);
		standardMat.mat_Metallic = 1.0f;
		standardMat.mat_Roughness = 0.2f;

		//Material* material = Renderer_CreateMaterial("Standard", "Standard");
		Material* material = Renderer_GetMaterial("Standard");
		ShaderInterop::SetStandardMaterialData(standardMat, material);

		material->Bind();

		/*
		CT_ERROR("PROJECTION!");
		CT_WARN("\n{}", projection.ToString());

		CT_ERROR("VIEW!");
		CT_WARN("\n{}", view.ToString());

		CT_ERROR("MODEL!");
		CT_WARN("\n{}", model.ToString());

		CT_ERROR("PROJECTION * VIEW * MODEL!");
		CT_WARN("\n{}", (projection * view * model).ToString());

		CT_ERROR("MODEL * VIEW * PROJECTION!");
		CT_WARN("\n{}", (model * view * projection).ToString());
		*/

		//glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA); // model matrix
		//glm::mat4 mvp = m_Proj * m_View * model;
		// m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), 
		// m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
		//cbt.transform = { {1, 3, 5, 7}, {2, 4, 6, 8}, {9, 11, 13, 15}, {10, 12, 14, 16} }; // WORKS PERFECTLY! (i think)

		UniformBufferDesc ubd = {};
		ubd.data = &cbt;
		ubd.dataBytes = sizeof(cbt);
		ubd.usage = Usage::Dynamic;

		UniformBuffer ub = m_Device->CreateUniformBuffer(&ubd);
        m_Device->RCBindUniformBuffer(&ub, ShaderType::Vertex, CTSI_CBSLOT_MATRICES); //TODO: Temp Metal
        m_Device->RCBindUniformBuffer(&ub, ShaderType::Fragment, CTSI_CBSLOT_MATRICES); //TODO: Temp Metal // for lighting!

		//cbt.transform = { {1, 3, 5, 7}, {2, 4, 6, 8}, {9, 11, 13, 15}, {10, 12, 14, 16} };
		m_Device->SetUniformBufferData(&ub, &cbt, sizeof(cbt));
		//cbt.transform = Math::Matrix4x4::Identity();
		//m_Device->SetUniformBufferData(&ub, &cbt, sizeof(cbt));

		// Vertex Buffer 1 Layout
        /*static */VertexBufferLayoutDesc vbld1 = {};
		vbld1.shader = &Renderer_GetShaders()["Standard"]; // TODO: heavily sure this is the reason why my materials don't work..

		vbld1.PushLayout("Position", 0, Format::R32G32B32_FLOAT);
		vbld1.PushLayout("Normal", 0, Format::R32G32B32_FLOAT);
		vbld1.PushLayout("TexCoord", 0, Format::R32G32_FLOAT);

        /*static */VertexBufferLayout vbLayout1 = m_Device->CreateVertexBufferLayout(&vbld1);
		//m_Device->BindVertexBufferLayout(&vbLayout1); // in pipeline

		// Pipeline
        /*static */PipelineState pipeline;
        //if (!pipeline.internal)
		{
            /*static */BlendStateDesc bsd;
			bsd.srcBlend = BlendFactor::SrcAlpha;
			bsd.destBlend = BlendFactor::OneMinusSrcAlpha;
			bsd.blendOperation = BlendOp::Add;

			bsd.renderTargetWriteMask = RenderTargetWriteMask::All;

            /*static */RasterizerStateDesc rsd;
			rsd.fillMode = FillMode::Solid;
			rsd.cullMode = CullMode::Back;
			rsd.frontCounterClockwise = false;

            /*static */DepthStencilStateDesc dsd;
			dsd.depthEnabled = true;
			dsd.depthWriteEnabled = true;
			dsd.depthFunc = DepthStencilComparisonFunc::Less;

            /*static */PipelineStateDesc psd = {};
			psd.blendState = &bsd;
			psd.rasterizerState = &rsd;
			psd.dsState = &dsd;
			psd.primitiveType = PrimitiveTopology::Triangles;

			psd.shader = &Renderer_GetShaders()["Standard"];
			psd.inputLayout = &vbLayout1;

			pipeline = m_Device->CreatePipelineState(&psd);
		}
        m_Device->RCBindPipelineState(&pipeline);

		// Vertex Buffer 1
		VertexBufferDesc vbd1 = {};
		vbd1.data = vertices.Data();
		vbd1.size = vertices.Size();
		vbd1.usage = Usage::Static;
		vbd1.vbLayoutDesc = &vbld1;

		VertexBuffer vbo1 = m_Device->CreateVertexBuffer(&vbd1);
		m_Device->RCBindVertexBuffer(&vbo1);

		// Texture
		if (!tex2D.internal)
		{
			Texture2DDesc td = {};
			{
				int width, height, channelsInFile;
				td.data = stbi_load(textureName.c_str(), &width, &height, &channelsInFile, 4); // D3D11 requires 4 channels! (RGBA)
				td.width = width;
				td.height = height;
			}
			td.format = Format::R8G8B8A8_U2FNORM;
			td.usage = Usage::Static;
			td.mipLevels = MIP_LEVELS_MAX;

			tex2D = m_Device->CreateTexture2D(&td);
			m_Device->RCBindTexture2D(&tex2D);

			stbi_image_free(td.data);
		}

		m_Device->RCBindTexture2D(&tex2D);

		/*FramebufferAttachments fba1; fba1 =
		{
			{FramebufferAttachmentType::Color, FramebufferFormat::Color},
			{FramebufferAttachmentType::DepthStencil, FramebufferFormat::Depth}
		};*/
		FramebufferAttachments fba1;
		fba1.PushAttachment(FramebufferAttachmentType::Color, FramebufferFormat::Color);
		fba1.PushAttachment(FramebufferAttachmentType::DepthStencil, FramebufferFormat::Depth);

		FramebufferDesc fbd1 = {};
		fbd1.width = 800;
		fbd1.height = 600;
		fbd1.attachments = &fba1;
		//fbd1.attachments =
		//{
		//	{FramebufferAttachmentType::Color, FramebufferFormat::Color},
		//	{FramebufferAttachmentType::DepthStencil, FramebufferFormat::Depth}
		//};
		//fbd1.attachments.attachments.PushBack(FramebufferAttachment{FramebufferAttachmentType::Color, FramebufferFormat::Color});
		//fbd1.attachments.attachments.PushBack(FramebufferAttachment{FramebufferAttachmentType::DepthStencil, FramebufferFormat::Depth});
		Framebuffer fbo1 = m_Device->CreateFramebuffer(&fbd1);

		//RenderPassDesc rpd1 = {};
		//rpd1.targetFramebuffer = &fbo1;
		//rpd1.clearColor = { 0.5f, 0.74f, 0.14f, 0.0f };
		//
		//RenderPass pass1 = m_Device->CreateRenderPass(&rpd1);
		//m_Device->RCBeginRenderPass(&pass1); //m_Device->SetTargetFramebuffer(&fbo1);

		//m_Device->RCDrawIndexed(ibo.GetCount());

		void* colorTexture = m_Device->GetFramebufferColorAttachment(&fbo1);
		//Image colorImage = m_Device->GetImageDataFromTexture(colorTexture);
		//for (Image::Pixel& pixel : colorImage.pixels)
		//{
		//	//CT_WARN("[{}, {}, {}, {}],\n", pixel.r, pixel.g, pixel.b, pixel.a);
		//}
		//
		//{
		//	CTL::DArray<uint8> pixels(800 * 600 * 4);
		//
		//	for (int i = 0; i < 800 * 600; ++i)
		//	{
		//		const auto& pixel = colorImage.pixels[i];
		//
		//		int index = i * 4;
		//		pixels[index + 0] = pixel.r; // Red
		//		pixels[index + 1] = pixel.g; // Green
		//		pixels[index + 2] = pixel.b; // Blue
		//		pixels[index + 3] = pixel.a; // Alpha
		//	}
		//
		//	// Save the image using stb_image_write
		//	int result = stbi_write_png("test_imagefb.png", 800, 600, 4, pixels.Data(), 800 * 4);
		//}

		const std::string matName = std::string("TestMaterial");
		///Material matTest;
		Material matTest(shader, &matName);
		float32 testData = 0.5f;
		matTest.PushProperty("u_Test", MaterialFormat::Float32, &testData);
		//matTest.UpdateData("u_Test", MaterialFormat::Float32, &testData);

		Math::Vector3 padding1;
		//matTest.PushProperty("padding1", MaterialFormat::Float32x3, &padding1); // implement something in the material system to not need this. It is done.

		Math::ColorF32x4 u_ColorData(1.0f, 0.2f, 0.5f);
		matTest.PushProperty("u_ColorData", MaterialFormat::Float32x4, &u_ColorData);
		matTest.SetProperty ("u_ColorData", MaterialFormat::Float32x4, &u_ColorData);
		u_ColorData.r = 0.5f;
		////matTest.UpdateData("u_ColorData", &u_ColorData, MaterialFormat::Float32x4);

		//struct MaterialTestBuffer
		//{
		//	MATERIAL_BUFFER_CLASS(MaterialTestBuffer);
		//
		//	Math::ColorF32x4 u_Color;
		//};
		//Material<MaterialTestBuffer> matTest;
		//matTest.GetBufferData()->u_Color = Math::ColorF32x4(1.0f, 0.2f, 0.5f);
		//matTest.UpdateData();

		//matTest.Bind(); // unbinded test
		{
			std::string source = JSON::SerializeObject(matTest, JSON::SerializerOptions(true));

			std::string filePath("Assets/Materials/");
			filePath.append(matTest.GetName());
			filePath.append(".ctmat");

			std::ofstream outFile(filePath);
			CT_CORE_ASSERT(outFile.is_open(), "Unable to open material file!");
			outFile << source;
			outFile.close();
		}
		//m_Device->BindShader(&shader);
		m_Device->RCDrawIndexed(ibo.GetCount());

		m_Device->RCEndRenderPass(); //m_Device->SetTargetFramebuffer(nullptr);
	}

	
	// ImGui
	void Renderer::ImGuiInit()
	{
		m_Device->ImGuiInitGraphicsAPI();
	}
    void Renderer::ImGuiNewFrame()
    {
        m_Device->ImGuiNewFrame();
    }
	void Renderer::ImGuiRenderDrawData(void* imDrawData)
	{
		m_Device->ImGuiRenderDrawData(imDrawData);
	}

	void Renderer::RenderMesh(const Mesh& mesh)
	{

	}
	void Renderer::RenderMeshWithMaterial(Mesh& mesh, Material& material)
	{
		// TODO: maybe not recreate all these objects every frame?
		material.Bind();
		
		CTL::DArray<float32> vertices(mesh.vertices.Size());
		for (const Vertex& vert : mesh.vertices)
		{
			vertices.PushBack(vert.position.x);
			vertices.PushBack(vert.position.y);
			vertices.PushBack(vert.position.z);

			vertices.PushBack(vert.normal.x);
			vertices.PushBack(vert.normal.y);
			vertices.PushBack(vert.normal.z);

			vertices.PushBack(vert.texCoord.u);
			vertices.PushBack(vert.texCoord.v);
		}

		// Index Buffer
		IndexBufferDesc ibd = {};
		ibd.data = mesh.indices.Data();
		ibd.count = mesh.indices.Count();

		IndexBuffer ibo = m_Device->CreateIndexBuffer(&ibd);
		m_Device->RCBindIndexBuffer(&ibo);

		VertexBufferLayoutDesc vbld1 = {};
		vbld1.shader = material.GetShader();

		vbld1.PushLayout("Position", 0, Format::R32G32B32_FLOAT);
		vbld1.PushLayout("Normal", 0, Format::R32G32B32_FLOAT);
		vbld1.PushLayout("TexCoord", 0, Format::R32G32_FLOAT);

		VertexBufferLayout vbLayout1 = m_Device->CreateVertexBufferLayout(&vbld1);
		//m_Device->BindVertexBufferLayout(&vbLayout1);

		// Vertex Buffer 1
		VertexBufferDesc vbd1 = {};
		vbd1.data = vertices.Data();
		vbd1.size = vertices.Size();
		vbd1.usage = Usage::Static;
		vbd1.vbLayoutDesc = &vbld1;

		VertexBuffer vbo1 = m_Device->CreateVertexBuffer(&vbd1);
		m_Device->RCBindVertexBuffer(&vbo1);

		m_Device->RCDrawIndexed(ibd.count);
	}

		/*struct VertexUBO
		{
			Math::Matrix4x4 VP;
			float GridSize;
			Math::Vector3 CameraWorldPos;
		};
		struct FragmentUBO
		{
			Math::Vector3 CameraWorldPos;
			float GridSize;
			float GridMinPixelsBetweenCells;
			float GridCellSize;
			Math::ColorF32x4 GridColorThin;
			Math::ColorF32x4 GridColorThick;
		};
		VertexUBO vertexUBData = {};
		vertexUBData.VP = Math::Matrix4x4::Identity();
		vertexUBData.GridSize = 100.0f;
		vertexUBData.CameraWorldPos = Math::Vector3::Zero();
		FragmentUBO fragUBData = {};
		fragUBData.CameraWorldPos = vertexUBData.CameraWorldPos;
		fragUBData.GridSize = vertexUBData.GridSize;
		fragUBData.GridMinPixelsBetweenCells = 2.0f;
		fragUBData.GridCellSize = 0.025f;
		fragUBData.GridColorThin = Math::ColorF32x4(0.5f, 0.5f, 0.5f, 1.0f);
		fragUBData.GridColorThick = Math::ColorF32x4(0.0f, 0.0f, 0.0f, 1.0f);

		UniformBufferDesc ubdVertex = {};
		ubdVertex.data = &vertexUBData;
		ubdVertex.dataBytes = sizeof(vertexUBData);
		ubdVertex.usage = Usage::Dynamic;

		UniformBuffer vertexUBO = m_Device->CreateUniformBuffer(&ubdVertex);

		UniformBufferDesc ubdFrag = {};
		ubdFrag.data = &fragUBData;
		ubdFrag.dataBytes = sizeof(fragUBData);
		ubdFrag.usage = Usage::Dynamic;

		UniformBuffer fragUBO = m_Device->CreateUniformBuffer(&ubdFrag);

		m_Device->BindUniformBuffer(&vertexUBO, ShaderType::Vertex);
		m_Device->BindUniformBuffer(&fragUBO, ShaderType::Fragment);*/

	void EditorRenderer::Initialize()
	{
		m_Device = RenderAPI::Device::Get();

		m_GridVertUBData.VP = Math::Matrix4x4::Identity();
		m_GridVertUBData.GridSize = 100.0f;
		m_GridVertUBData.CameraWorldPos = Math::Vector3::Zero();

		m_GridFragUBData.CameraWorldPos = m_GridVertUBData.CameraWorldPos;
		m_GridFragUBData.GridSize = m_GridVertUBData.GridSize;
		m_GridFragUBData.GridMinPixelsBetweenCells = 2.0f;
		m_GridFragUBData.GridCellSize = 0.025f;
		m_GridFragUBData.GridColorThin = Math::ColorF32x4(0.5f, 0.5f, 0.5f, 1.0f); // Math::ColorF32x4(1.5f, 0.5f, 0.5f, 1.0f)
		m_GridFragUBData.GridColorThick = Math::ColorF32x4(0.25f, 0.25f, 0.25f, 1.0f);

		UniformBufferDesc ubdVertex = {};
		ubdVertex.data = &m_GridVertUBData;
		ubdVertex.dataBytes = sizeof(m_GridVertUBData);
		ubdVertex.usage = Usage::Dynamic;

		m_GridVertUB = m_Device->CreateUniformBuffer(&ubdVertex);

		UniformBufferDesc ubdFrag = {};
		ubdFrag.data = &m_GridFragUBData;
		ubdFrag.dataBytes = sizeof(m_GridFragUBData);
		ubdFrag.usage = Usage::Dynamic;

		m_GridFragUB = m_Device->CreateUniformBuffer(&ubdFrag);

		ShaderDesc grsd = {};
		grsd.name = "InfiniteGrid";

		m_GridShader = &Renderer_GetShaders()[grsd.name];

		// Pipeline State Object
		VertexBufferLayoutDesc vbild = {}; // Grid shader does not require any input
		vbild.shader = m_GridShader;
		static VertexBufferLayout il = m_Device->CreateVertexBufferLayout(&vbild);

		BlendStateDesc bsd;
		bsd.srcBlend = BlendFactor::SrcAlpha;
		bsd.destBlend = BlendFactor::OneMinusSrcAlpha;
		bsd.blendOperation = BlendOp::Add;

		bsd.renderTargetWriteMask = RenderTargetWriteMask::All;

		RasterizerStateDesc rsd;
		rsd.fillMode = FillMode::Solid;
		rsd.cullMode = CullMode::None;
		rsd.frontCounterClockwise = false;

		DepthStencilStateDesc dsd;
		dsd.depthEnabled = true;
		dsd.depthWriteEnabled = true; // false; // stops clipping. at a cost..
		dsd.depthFunc = DepthStencilComparisonFunc::Less;

		PipelineStateDesc psd = {};
		psd.blendState = &bsd;
		psd.rasterizerState = &rsd;
		psd.dsState = &dsd;
		psd.primitiveType = PrimitiveTopology::Triangles;

		psd.shader = m_GridShader;
		psd.inputLayout = &il;

		m_GridPipeline = m_Device->CreatePipelineState(&psd); // TODO: why isn't the grid as transparent as it should be? probably because transparent objects need to be rendered last...
	}
	void EditorRenderer::Render(Camera* camera, Math::Transform* camTransform) // TODO: maybe separate grid rendering?
	{
		CT_PROFILE_STATIC_FUNCTION(Renderer);

		Device_PushDebugGroup("Editor Grid Render");

		m_Device->RCBindUniformBuffer(&m_GridVertUB, ShaderType::Vertex, 0);
		m_Device->RCBindUniformBuffer(&m_GridFragUB, ShaderType::Fragment, 0);

		m_GridVertUBData.VP = camera->GetProjection() * camTransform->GetCameraViewFromTransform();
		m_GridVertUBData.CameraWorldPos = m_GridFragUBData.CameraWorldPos = camTransform->position;
        //m_GridFragUBData.padding1[0] = 69;
        //m_GridFragUBData.padding1[7] = 69;

		m_Device->SetUniformBufferData(&m_GridVertUB, &m_GridVertUBData, sizeof(m_GridVertUBData));
		m_Device->SetUniformBufferData(&m_GridFragUB, &m_GridFragUBData, sizeof(m_GridFragUBData));

		m_Device->RCBindPipelineState(&m_GridPipeline);

        //VertexBufferDesc vbd;
        //uint8 emptyData[96];
        //vbd.data = &emptyData;
        //vbd.size = sizeof(emptyData);
        //VertexBuffer emptyBuffer = m_Device->CreateVertexBuffer(&vbd);
        //m_Device->RCBindVertexBuffer(&emptyBuffer);
		m_Device->RCDraw(6);

		Device_PopDebugGroup();
	}
}
