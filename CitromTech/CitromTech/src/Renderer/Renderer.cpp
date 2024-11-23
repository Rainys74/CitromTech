#include "Renderer.h"

namespace Citrom
{
	CTL::ScopedPtr<RenderAPI::Device> Renderer::m_Device;

	void Renderer::Initialize()
	{
		m_Device = CTL::CreateScopedPtr<RenderAPI::Device>();
	}

	void Renderer::DrawTest()
	{
	}
}