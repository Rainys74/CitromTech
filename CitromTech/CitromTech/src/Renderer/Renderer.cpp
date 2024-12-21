#include "Renderer.h"

#include "Renderer/RenderAPI/Buffer.h"

namespace Citrom
{
	using namespace RenderAPI;

	CTL::ScopedPtr<RenderAPI::Device> Renderer::m_Device;

	void Renderer::Initialize()
	{
		GraphicsAPIManager::ForceGraphicsAPI(GraphicsAPI::DirectX11);

		m_Device = CTL::CreateScopedPtr<RenderAPI::Device>();
	}

	void Renderer::DrawTest()
	{
	}
}