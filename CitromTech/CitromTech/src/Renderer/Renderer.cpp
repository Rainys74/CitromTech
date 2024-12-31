#include "Renderer.h"

#include "Renderer/RenderAPI/Buffer.h"

namespace Citrom
{
	using namespace RenderAPI;

	RenderAPI::Device* Renderer::m_Device;

	void Renderer::Initialize()
	{
		GraphicsAPIManager::ForceGraphicsAPI(GraphicsAPI::DirectX11);
		//GraphicsAPIManager::ForceGraphicsAPI(GraphicsAPI::OpenGL);

		//m_Device = CTL::CreateScopedPtr<RenderAPI::Device>();
		m_Device = Device::Get();
	}

	void Renderer::DrawTest()
	{
	}

	
	// ImGui
	void Renderer::ImGuiInit()
	{
		m_Device->ImGuiInitGraphicsAPI();
	}
	void Renderer::ImGuiRenderDrawData()
	{
		m_Device->ImGuiRenderDrawData();
	}
}