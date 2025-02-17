#pragma once

#include "Graphics.h"

#include "CTL/RefPointer.h"

namespace Citrom::RenderAPI
{
	struct Texture2DDesc
	{
		uint8* data;
		uint32 width, height;

		Usage usage;
		Format format = Format::R8G8B8A8_U2FNORM;

		uint32 arraySize = 1; // if it weren't for backwards-compatibility, this should be named as arrayCount or arrayLength.
		uint32 mipLevels = 1;

		struct SamplerDesc
		{
			bool test;
		} sampler;
	};

	// TODO: combined samplers or separated? thinking about making a SamplerDesc for use for framebuffers and textures, however samplers would be created in framebuffer or texture code and not separately..
	struct Texture2D
	{
		Texture2DDesc descriptor;
		CTL::Ref<void> internal;
	};
}
