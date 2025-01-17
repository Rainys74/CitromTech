#include "DX11Device.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11DebugHandler.h"

namespace Citrom::RenderAPI
{
	void DX11Device::RCClearColor(float32 r, float32 g, float32 b, float32 a)
	{
		const float colors[] = { r, g, b, a };
		DXCall(m_DeviceContext->ClearRenderTargetView(m_RenderTarget, colors));
	}
	void DX11Device::RCDrawIndexed(uint32 indexCount)
	{
		DXCall(m_DeviceContext->DrawIndexed(indexCount, 0, 0));
	}
}
#endif