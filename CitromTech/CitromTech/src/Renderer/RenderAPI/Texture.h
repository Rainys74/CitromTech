#pragma once

#include "Graphics.h"

#include "CTL/RefPointer.h"

namespace Citrom::RenderAPI
{
	struct Texture2DDesc
	{
		uint8* data;
		uint32 width, height;

		Format format = Format::R8G8B8A8_U2FNORM;

		uint32 arraySize = 1;
		uint32 mipLevels = 1;
	};

	// TODO: combined samplers or separated?
	struct Texture2D
	{
		Texture2DDesc descriptor;
		CTL::Ref<void> internal;
	};
}