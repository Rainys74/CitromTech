#include "DX11Device.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11DebugHandler.h"

namespace Citrom::RenderAPI
{
	void DX11Device::RCClearColor(float32 r, float32 g, float32 b, float32 a)
	{
		const float colors[] = { r, g, b, a };
		m_DeviceContext->ClearRenderTargetView(m_RenderTarget, colors);
	}
}
#endif