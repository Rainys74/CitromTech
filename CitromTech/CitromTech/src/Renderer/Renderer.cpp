#include "Renderer.h"

#include "Renderer/RenderAPI/Buffer.h"
#include "Vendor/stb/stb_image_write.h"

#include "EntitySystem/Components/EssentialComponents.h"
#include "EntitySystem/Components/RendererComponents.h"
#include "EntitySystem/Entity.h"

#include "Math/Vector.h"

namespace Citrom
{
	using namespace RenderAPI;

	RenderAPI::Device* Renderer::m_Device;
	EventListener<WindowEvents> Renderer::s_WindowEventListener;

	void Renderer::Initialize(Platform::Window* window)
	{
		GraphicsAPIManager::ForceGraphicsAPI(GraphicsAPI::DirectX11);
		//GraphicsAPIManager::ForceGraphicsAPI(GraphicsAPI::OpenGL);
		
		CT_TRACE("CURRENT API: {}", (size_t)GraphicsAPIManager::GetGraphicsAPI());
		for (size_t i = 0; i < (size_t)GraphicsAPI::Count; i++)
		{
			CT_TRACE("{}. {}", i, (size_t)GraphicsAPIManager::GetGraphicsAPIAtPriority(i));
		}

		//m_Device = CTL::CreateScopedPtr<RenderAPI::Device>();
		m_Device = Device::Get();

		SwapChainDesc scd;
		scd.windowPtr = window;

		m_Device->SetVSync(VSyncMode::On);
		m_Device->MakeSwapChain(&scd);

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
		CT_CORE_INFO("\tTotal VRAM: {} MB", gpuInfo.videoMemory / (1 << 20)); // 1024 * 1024, 1 << 20, 1e+6

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

	void Renderer::DrawTest()
	{
		CT_PROFILE_STATIC_FUNCTION(Renderer);

		float positions[] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f
		};

		unsigned int indices[] =
		{
			0, 1, 2//,
			//2, 3, 0
		};

		// Index Buffer
		IndexBufferDesc ibd = {};
		ibd.data = indices;
		ibd.count = 3;

		IndexBuffer ibo = m_Device->CreateIndexBuffer(&ibd);
		m_Device->BindIndexBuffer(&ibo);

		// Shader
		ShaderDesc sd = {};
		sd.name = "Standard";

		Shader shader = m_Device->CreateShader(&sd);
		m_Device->BindShader(&shader);

		// Vertex Buffer 1 Layout
		VertexBufferLayoutDesc vbld1 = {};
		vbld1.shader = &shader;

		vbld1.PushLayout("Position", 0, Format::R32G32B32_FLOAT);

		VertexBufferLayout vbLayout1 = m_Device->CreateVertexBufferLayout(&vbld1);
		m_Device->BindVertexBufferLayout(&vbLayout1);

		// Vertex Buffer 1
		VertexBufferDesc vbd1 = {};
		vbd1.data = positions;
		vbd1.size = sizeof(positions);
		vbd1.usage = Usage::Static;
		vbd1.vbLayoutDesc = &vbld1;

		VertexBuffer vbo1 = m_Device->CreateVertexBuffer(&vbd1);
		m_Device->BindVertexBuffer(&vbo1);

		FramebufferDesc fbd1 = {};
		fbd1.width = 800;
		fbd1.height = 600;
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
}