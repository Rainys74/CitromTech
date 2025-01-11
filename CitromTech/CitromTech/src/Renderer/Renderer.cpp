#include "Renderer.h"

#include "Renderer/RenderAPI/Buffer.h"

namespace Citrom
{
	using namespace RenderAPI;

	RenderAPI::Device* Renderer::m_Device;
	EventListener<WindowEvents> Renderer::s_WindowEventListener;

	void Renderer::Initialize(Platform::Window* window)
	{
		GraphicsAPIManager::ForceGraphicsAPI(GraphicsAPI::DirectX11);
		//GraphicsAPIManager::ForceGraphicsAPI(GraphicsAPI::OpenGL);

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

		std::string shaderPath("Shaders/");
		std::string shaderCachePath("ShaderCache/");

		ShaderCompiler::PrepareShaders(&shaderPath, 1, shaderCachePath);
		ShaderCompiler::CompileShaders(&shaderCachePath, 1, shaderCachePath);
	}

	void Renderer::BeginFrame()
	{
		m_Device->RCClearColor(0.5f, 0.74f, 0.14f);
	}

	void Renderer::EndFrame()
	{
		m_Device->SwapBuffers();
	}

	void Renderer::DrawTest()
	{

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
}