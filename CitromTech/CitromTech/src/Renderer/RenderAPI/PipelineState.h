#pragma once

#include "Graphics.h"
#include "Platform/PlatformWindow.h"

namespace Citrom::RenderAPI
{
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

		Red = BIT(0),
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

	enum class FillMode
	{
		Solid, // Fill
		Wireframe // Line
	};
	enum class CullMode
	{
		Back,
		Front
	};

	struct RasterizerStateDesc
	{
		FillMode fillMode = FillMode::Solid;
		CullMode cullMode = CullMode::Back;

		bool frontCounterClockwise = false;
	};

	// TODO: move to pipelines since they're much more modern and hold up better with api's like vulkan and metal, also easier to performantly implement for me ;)
	struct PipelineStateDesc
	{
		const Shader* vertexShader;
		const Shader* fragmentShader;
		//const ComputeShader* computeShader;

		//const BlendState* blendState;
		//const RasterizerState* rasterizerState;
		//const DepthStencilState* dsState;

		//const VertexBufferLayout* inputLayout;

		//PrimitiveTopology primitiveType;
		// Sample Mask?
	};

	struct PipelineState
	{
		PipelineStateDesc descriptor;
		CTL::Ref<void> internal;
	};
}