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

	enum class BlendOp
	{
		Add,
		Subtract,
		ReverseSubtract,
		Min,
		Max
	};
	enum class BlendFactor
	{
		Zero,
		One,
		SrcAlpha,
		OneMinusSrcAlpha,
		//DstAlpha,
		//OneMinusDstAlpha
	};
	enum class RenderTargetWriteMask : uint8fast
	{
		None = 0,
		Red = 1,
		Green = BIT(1),
		Blue = BIT(2),
		Alpha = BIT(3),

		All = Red | Green | Blue | Alpha
	};

	struct BlendStateDesc
	{
		/*struct BlendFactorMultiplier
		{
			float32 r, g, b, a;
		} blendFactorMult;*/
		
		BlendFactor srcBlend = BlendFactor::One;
		BlendFactor destBlend = BlendFactor::Zero;
		BlendOp blendOperation = BlendOp::Add;

		BlendFactor srcBlendAlpha = BlendFactor::SrcAlpha;
		BlendFactor destBlendAlpha = BlendFactor::OneMinusSrcAlpha;
		BlendOp blendOperationAlpha = BlendOp::Add;

		RenderTargetWriteMask renderTargetWriteMask = RenderTargetWriteMask::All;
	};
}