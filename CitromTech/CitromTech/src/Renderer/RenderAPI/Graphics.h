#pragma once

#include "Core.h"

#include "CTL/String.h"

namespace Citrom::RenderAPI
{
	enum class GraphicsAPI
	{
		DirectX11,
		OpenGL, // Someday
        Metal,
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
		Adaptive = -1,

		Off = 0,
		On = 1, // Every V-Blank
		Half = 2, // Every Second V-Blank

		Quarter = 3, // Every Third V-Blank
		Eighth = 4 // Every Fourth V-Blank
	};

	enum class ShaderType
	{
		Vertex,
		Fragment, // Pixel
		Compute // Kernel
	};
	enum class ShaderLanguage
	{
		None = 0,

		HLSL = BIT(0),
		GLSL = BIT(1),
		MSL = BIT(2),

		// Compiled
		DXBC = BIT(3), // HLSL5
		//SPIRV = BIT(4),

		// Defaults
		Text = HLSL | GLSL | MSL,
		Compiled = DXBC,

		All = Text | Compiled
	};

	enum class Format
	{
		Unknown,

		// Float
		R32_FLOAT, // 1
		R32G32_FLOAT, // 2
		R32G32B32_FLOAT, // 3
		R32G32B32A32_FLOAT, // 4

		// Unsigned Normalized (UNORM) [0, MaxUIntValue] to [0.0f, 1.0f] TODO: think about replacing U2FNORM to U2F
		R8_U2FNORM, // 1
		R8G8_U2FNORM, // 2
		R8G8B8A8_U2FNORM, // 4
		B8G8R8A8_U2FNORM, // 4

		Count
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

	struct Image
	{
		struct Pixel
		{
			uint8 r, g, b, a;
		};
		CTL::DArray<Pixel> pixels;
	};

	// width and height is a multiplier of the screen size, so is position
	struct ViewportSpecification
	{
		ViewportSpecification() = default;
		ViewportSpecification(float32 _width, float32 _height, float32 _xPos = 0.0f, float32 _yPos = 0.0f, float32 _zNear = 0.0f, float32 _zFar = 1.0f)
			: width(_width), height(_height), xPos(_xPos), yPos(_yPos), zNear(_zNear), zFar(_zFar) {}

		float32 width = 1.0f;
		float32 height = 1.0f;

		float32 xPos = 0.0f; // position starts from bottom left
		float32 yPos = 0.0f;

		float32 zNear = 0.0f;
		float32 zFar = 1.0f;
	};
}
