#pragma once

#include "Core.h"

namespace Citrom::RenderAPI
{
	enum class GraphicsAPI
	{
		DirectX11,
		OpenGL, // Someday
		//Vulkan, // hopefully someday

		Count
	};

	enum class Usage
	{
		Static,
		Dynamic
	};
}