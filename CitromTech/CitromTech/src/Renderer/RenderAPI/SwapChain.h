#pragma once

#include "Graphics.h"
#include "Platform/PlatformWindow.h"

namespace Citrom::RenderAPI
{
	struct SwapChainDesc
	{
		Platform::Window* windowPtr;
		Format renderFormat;
	};
}