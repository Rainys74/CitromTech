#pragma once

#include "Core.h"

#include "CTL/String.h"

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

	enum class VSyncMode //VSyncType
	{
		//Adaptive = -1,

		Off = 0,
		On = 1, // Every V-Blank
		Half = 2, // Every Second V-Blank

		Quarter = 3, // Every Third V-Blank
		Eighth = 4 // Every Fourth V-Blank
	};

	enum class ShaderType
	{
		Vertex,
		Fragment // Pixel
	};

	struct GPUInfo
	{
		std::string vendor;
		std::string renderer;
		std::string version;
		std::string shadingLanguageVersion;
		// Extensions?

		size_t videoMemory; // VRAM in bytes
	};
}