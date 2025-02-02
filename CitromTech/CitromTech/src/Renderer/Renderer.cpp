#include "Renderer.h"
#include "ArgumentHandler.h"

#include "Renderer/RenderAPI/Buffer.h"
#include "Vendor/stb/stb_image_write.h"
#include "Vendor/stb/stb_image.h"
#include "Vendor/tinyobjloader/tiny_obj_loader.h"

#include "EntitySystem/Components/EssentialComponents.h"
#include "EntitySystem/Components/RendererComponents.h"
#include "EntitySystem/Entity.h"

#include "Math/Vector.h"

namespace Citrom
{
	using namespace RenderAPI;

	RenderAPI::Device* Renderer::m_Device;
	EventListener<WindowEvents> Renderer::s_WindowEventListener;

	static EditorRenderer g_EditorRenderer;

	void Renderer::Initialize(Platform::Window* window)
	{
		GraphicsAPIManager::ForceGraphicsAPI(GraphicsAPI::DirectX11);
		//GraphicsAPIManager::ForceGraphicsAPI(GraphicsAPI::OpenGL);

		if (ArgumentHandler::HasArgument("-force-d3d11"))
			GraphicsAPIManager::ForceGraphicsAPI(GraphicsAPI::DirectX11);
		
		CT_TRACE("CURRENT API: {}", (size_t)GraphicsAPIManager::GetGraphicsAPI());
		for (size_t i = 0; i < (size_t)GraphicsAPI::Count; i++)
		{
			CT_TRACE("{}. {}", i, (size_t)GraphicsAPIManager::GetGraphicsAPIAtPriority(i));
		}

		//m_Device = CTL::CreateScopedPtr<RenderAPI::Device>();
		m_Device = Device::Get();

		SwapChainDesc scd;
		scd.windowPtr = window;
		scd.renderFormat = Format::B8G8R8A8_U2FNORM;

		BlendStateDesc bsd;
		bsd.srcBlend = BlendFactor::SrcAlpha;
		bsd.destBlend = BlendFactor::OneMinusSrcAlpha;
		bsd.blendOperation = BlendOp::Add;

		bsd.renderTargetWriteMask = RenderTargetWriteMask::All;

		RasterizerStateDesc rsd;
		rsd.fillMode = FillMode::Solid;
		rsd.cullMode = CullMode::Back;
		rsd.frontCounterClockwise = false;

		m_Device->SetVSync(VSyncMode::On);
		m_Device->MakeSwapChain(&scd, &bsd, &rsd);

		// On Resize Callback
		s_WindowEventListener.OnEvent = [](const Event<WindowEvents>& event)
		{
			if (event.GetEventType() == WindowEvents::WindowResize)
			{
				const WindowResizeEvent& transformedEvent = (const WindowResizeEvent&)event;
				Device::Get()->Resize(transformedEvent.width, transformedEvent.height);
			}
		};
		EventBus::GetDispatcher<WindowEvents>()->AddListener(&s_WindowEventListener);

		GPUInfo gpuInfo = m_Device->GetCurrentGPUInfo();
		CT_CORE_INFO("Graphics Adapter Info:");
		CT_CORE_INFO("\tVendor: {}", gpuInfo.vendor);
		CT_CORE_INFO("\tRenderer: {}", gpuInfo.renderer);
		CT_CORE_INFO("\tVersion: {}", gpuInfo.version);
		CT_CORE_INFO("\tShading Language Version: {}", gpuInfo.shadingLanguageVersion);
		CT_CORE_INFO("\tTotal VRAM: {} MB", gpuInfo.videoMemory / (1 << 20)); // 1024 * 1024, 1 << 20, 1e+6 (10 ^ +6 = 1'000'000)

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

		ShaderCompiler::ClearShaderCache("ShaderCache/");

		// TODO: 2 might need to be changed to 3, as well as shaders might need
		// to be transpiled after compilation if i plan on implementing HLSLcc
		ShaderCompiler::PrepareShaders(shaderPaths, 2, "ShaderCache/");
		ShaderCompiler::CompileShaders(shaderPaths, 3, "ShaderCache/");

		g_EditorRenderer.Initialize();
	}

	void Renderer::BeginFrame()
	{
		//m_Device->RCClearColor(1.0f, 0.5f, 0.24f, 1.0f); // Citrom Tech 2D
		m_Device->RCClearColor(0.5f, 0.74f, 0.14f); // Citrom Tech
	}

	void Renderer::EndFrame()
	{
		m_Device->SwapBuffers();
	}

	void Renderer::Begin()
	{
	}

	void Renderer::End()
	{
	}

	void Renderer::SubmitScene(Scene* scene)
	{
		// TODO: make sure it uses submittion
		// Right now it draws immediately
		auto view = scene->GetAllEntitiesWith<CubeComponent>();
		for (auto cube : view)
		{
			//auto& transformComponent = view.get<TransformComponent>(cube);
			//auto& cubeComponent = view.get<CubeComponent>(cube);

			static constexpr float32 positions[] =
			{
				// Front face
				-0.5f, -0.5f,  0.5f, // Bottom-left
				 0.5f, -0.5f,  0.5f, // Bottom-right
				 0.5f,  0.5f,  0.5f, // Top-right
				-0.5f,  0.5f,  0.5f, // Top-left

				// Back face
				-0.5f, -0.5f, -0.5f, // Bottom-left
				 0.5f, -0.5f, -0.5f, // Bottom-right
				 0.5f,  0.5f, -0.5f, // Top-right
				-0.5f,  0.5f, -0.5f, // Top-left
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

			for (size_t i = 0; i < CT_ARRAY_LENGTH(positions) / 3; i++)
			{
				Vertex v;
				v.position = Math::Vector3({ positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2] });

				cubeMesh.vertices.PushBack(v);
			}

			for (size_t i = 0; i < CT_ARRAY_LENGTH(indices); i++)
				cubeMesh.indices.PushBack(indices[i]);

			RenderMesh(cubeMesh);
		}
	}

	void Renderer::DrawTest(Camera* camera, Math::Transform* cameraTransform)
	{
		CT_PROFILE_STATIC_FUNCTION(Renderer);

		g_EditorRenderer.Render(camera, cameraTransform);

		//static CTL::DArray<float> vertices;
		static CTL::DArray<Vertex> vertexes;
		static CTL::DArray<unsigned int> indices;
		static std::string textureName = "texture.png";

		float triangleVertices[] =
		{
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
			 0.0f,  0.5f, 0.0f,		0.5f, 1.0f,
			 0.5f, -0.5f, 0.0f,		1.0f, 0.0f
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

			vertexes.Reserve(attrib.vertices.size() / 3);
			for (size_t v = 0; v < attrib.vertices.size() / 3; v++)
				vertexes.PushBack(Vertex{});

			for (size_t v = 0; v < attrib.vertices.size() / 3; v++)
			{
				vertexes[v].position.x = attrib.vertices[3 * v + 0];
				vertexes[v].position.y = attrib.vertices[3 * v + 1];
				vertexes[v].position.z = attrib.vertices[3 * v + 2];
			}

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

		CTL::DArray<float> vertices;
		for (const Vertex& vert : vertexes)
		{
			vertices.PushBack(vert.position.x);
			vertices.PushBack(vert.position.y);
			vertices.PushBack(vert.position.z);

			vertices.PushBack(vert.texCoord.u);
			vertices.PushBack(vert.texCoord.v);
		}

		// Index Buffer
		IndexBufferDesc ibd = {};
		ibd.data = indices.Data();
		ibd.count = indices.Count();

		IndexBuffer ibo = m_Device->CreateIndexBuffer(&ibd);
		m_Device->BindIndexBuffer(&ibo);

		// Shader
		ShaderDesc sd = {};
		sd.name = "Standard";

		Shader shader = m_Device->CreateShader(&sd);
		m_Device->BindShader(&shader);

		// Shader Constant Buffer
		struct ConstantBufferTest
		{
			Math::Matrix4x4 transform;
		};
		ConstantBufferTest cbt = {};
		cbt.transform = Math::Matrix4x4::Identity();
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
		Math::Matrix4x4 model = Math::Matrix4x4::Translate(Math::Matrix4x4::Identity(), Math::Vector3(0.0f, 0.0f, 0.0f));
		cbt.transform = projection * view * model;
		//cbt.transform = model * view * projection; // INCORRECT!
		cbt.transform.Transpose(); // TODO: what to do to not need this? because without it some weird looking artifacts appear. About ~0.002 ms worth of overhead

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
		m_Device->BindUniformBuffer(&ub);

		//cbt.transform = { {1, 3, 5, 7}, {2, 4, 6, 8}, {9, 11, 13, 15}, {10, 12, 14, 16} };
		//m_Device->SetUniformBufferData(&ub, &cbt, sizeof(cbt));
		//cbt.transform = Math::Matrix4x4::Identity();
		//m_Device->SetUniformBufferData(&ub, &cbt, sizeof(cbt));

		// Vertex Buffer 1 Layout
		VertexBufferLayoutDesc vbld1 = {};
		vbld1.shader = &shader;

		vbld1.PushLayout("Position", 0, Format::R32G32B32_FLOAT);
		vbld1.PushLayout("TexCoord", 1, Format::R32G32_FLOAT);

		VertexBufferLayout vbLayout1 = m_Device->CreateVertexBufferLayout(&vbld1);
		m_Device->BindVertexBufferLayout(&vbLayout1);

		// Vertex Buffer 1
		VertexBufferDesc vbd1 = {};
		vbd1.data = vertices.Data();
		vbd1.size = vertices.Size();
		vbd1.usage = Usage::Static;
		vbd1.vbLayoutDesc = &vbld1;

		VertexBuffer vbo1 = m_Device->CreateVertexBuffer(&vbd1);
		m_Device->BindVertexBuffer(&vbo1);

		// Texture
		Texture2DDesc td = {};
		{
			int width, height, channelsInFile;
			td.data = stbi_load(textureName.c_str(), &width, &height, &channelsInFile, 4); // D3D11 requires 4 channels! (RGBA)
			td.width = width;
			td.height = height;
		}
		td.format = Format::R8G8B8A8_U2FNORM;
		td.usage = Usage::Static;

		Texture2D tex2D = m_Device->CreateTexture2D(&td);
		m_Device->BindTexture2D(&tex2D);

		stbi_image_free(td.data);

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

		//m_Device->SetTargetFramebuffer(&fbo1);

		m_Device->RCDrawIndexed(ibo.GetCount());

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

		//Material matTest;
		////float32 testData = 0.5f;
		////matTest.PushProperty("u_Test", MaterialFormat::Float32, &testData);
		////matTest.UpdateData("u_Test", &testData, MaterialFormat::Float32);
		////
		////Math::Vector3 padding1;
		////matTest.PushProperty("padding1", MaterialFormat::Float32x3, &padding1);

		//Math::ColorF32x4 u_ColorData(1.0f, 0.2f, 0.5f);
		//matTest.PushProperty("u_ColorData", MaterialFormat::Float32x4, &u_ColorData);
		//matTest.UpdateData("u_ColorData", &u_ColorData, MaterialFormat::Float32x4);
		//u_ColorData.r = 0.5f;
		////matTest.UpdateData("u_ColorData", &u_ColorData, MaterialFormat::Float32x4);

		//m_Device->RCDrawIndexed(ibo.GetCount());

		m_Device->SetTargetFramebuffer(nullptr);
	}

	
	// ImGui
	void Renderer::ImGuiInit()
	{
		m_Device->ImGuiInitGraphicsAPI();
	}
	void Renderer::ImGuiRenderDrawData(void* imDrawData)
	{
		m_Device->ImGuiRenderDrawData(imDrawData);
	}

	void Renderer::RenderMesh(const Mesh& mesh)
	{

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

		m_GridShader = m_Device->CreateShader(&grsd);
	}
	void EditorRenderer::Render(Camera* camera, Math::Transform* camTransform)
	{
		CT_PROFILE_STATIC_FUNCTION(Renderer);

		m_Device->BindUniformBuffer(&m_GridVertUB, ShaderType::Vertex, 0);
		m_Device->BindUniformBuffer(&m_GridFragUB, ShaderType::Fragment, 1);

		m_GridVertUBData.VP = camera->GetProjection() * camTransform->GetCameraViewFromTransform();
		m_GridVertUBData.CameraWorldPos = m_GridFragUBData.CameraWorldPos = camTransform->position;

		m_Device->SetUniformBufferData(&m_GridVertUB, &m_GridVertUBData, sizeof(m_GridVertUBData));
		m_Device->SetUniformBufferData(&m_GridFragUB, &m_GridFragUBData, sizeof(m_GridFragUBData));

		m_Device->BindShader(&m_GridShader);

		m_Device->RCDraw(6);
	}
}