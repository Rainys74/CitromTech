#pragma once

#include "Renderer/RenderAPI/GraphicsDevice.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11Includes.h"

namespace Citrom::RenderAPI
{
	class DX11Device : public Device
	{

	};
}
#endif