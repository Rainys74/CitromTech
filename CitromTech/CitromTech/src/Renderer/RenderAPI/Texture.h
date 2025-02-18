#pragma once

#include "Graphics.h"

#include "CTL/RefPointer.h"

namespace Citrom::RenderAPI
{
	enum class TextureAddressMode
	{
		Wrap, // GL_REPEAT = D3D11_TEXTURE_ADDRESS_WRAP = MTLSamplerAddressModeRepeat
		Mirror, // GL_MIRRORED_REPEAT = D3D11_TEXTURE_ADDRESS_MIRROR = MTLSamplerAddressModeMirrorRepeat
		Clamp, // GL_CLAMP_TO_EDGE = D3D11_TEXTURE_ADDRESS_CLAMP = MTLSamplerAddressModeClampToEdge
		Border, // GL_CLAMP_TO_BORDER = D3D11_TEXTURE_ADDRESS_BORDER = MTLSamplerAddressModeClampToBorderColor?
		MirrorOnce // D3D11_TEXTURE_ADDRESS_MIRROR_ONCE = MTLSamplerAddressModeMirrorClampToEdge?
	};
	enum class TextureFilterMode
	{
		Point,
		Linear,

		// Defaults
		Nearest = Point,
		NearestNeighbor = Point,
		NearestNeighbour = Point
	};

	struct SamplerDesc
	{
		union
		{
			struct
			{
				TextureAddressMode addressU;
				TextureAddressMode addressV;
				TextureAddressMode addressW;
			};
			struct
			{
				TextureAddressMode addressS;
				TextureAddressMode addressT;
				TextureAddressMode addressR;
			};
		};
	};

#define MIP_LEVELS_NONE (1)
#define MIP_LEVELS_MAX (0) // ANY

	struct Texture2DDesc
	{
		uint8* data;
		uint32 width, height;

		Usage usage;
		Format format = Format::R8G8B8A8_U2FNORM;

		uint32 arraySize = 1; // if it weren't for backwards-compatibility, this should be named as arrayCount or arrayLength.
		uint32 mipLevels = MIP_LEVELS_NONE;

		SamplerDesc sampler;
	};

	// TODO: combined samplers or separated? thinking about making a SamplerDesc for use for framebuffers and textures, however samplers would be created in framebuffer or texture code and not separately..
	struct Texture2D
	{
		Texture2DDesc descriptor;
		CTL::Ref<void> internal;
	};
}
