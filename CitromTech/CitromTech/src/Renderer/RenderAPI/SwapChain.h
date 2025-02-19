#pragma once

#include "Graphics.h"
#include "Platform/PlatformWindow.h"

namespace Citrom::RenderAPI
{
	struct SwapChainDesc
	{
		Platform::Window* windowPtr;
		Format renderFormat;
		//FramebufferFormat depthStencilFormat; // None/D16/D32F/DEPTH24STENCIL8 (DEPTH24NSTENCIL8U)
	};
}