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
		None,
		Back,
		Front
	};

	struct RasterizerStateDesc
	{
		FillMode fillMode = FillMode::Solid;
		CullMode cullMode = CullMode::Back;

		bool frontCounterClockwise = false;

		//bool conservativeRasterization = false; // unsupported for DirectX 11.0
	};

	enum class DepthStencilComparisonFunc
	{
		Never,			// 0

		Equal,			// ==
		NotEqual,		// !=
		Less,			// <
		LessEqual,		// <=
		Greater,		// >
		GreaterEqual,	// >=

		Always			// 1
	};

	enum class StencilOp
	{
		Keep,
		Zero,
		Replace,
		IncrementAndClamp,
		DecrementAndClamp,
		Invert,
		IncrementAndWrap,
		DecrementAndWrap
	};
	struct StencilOpListDesc
	{
		StencilOp stencilDepthFailOp;
		StencilOp stencilPassDepthFailOp;
		StencilOp stencilDepthPassOp;
		DepthStencilComparisonFunc stencilFunc;
	};

	struct DepthStencilStateDesc
	{
		bool depthEnabled = true;
		bool depthWriteEnabled = true;
		DepthStencilComparisonFunc depthFunc = DepthStencilComparisonFunc::Less;

		bool stencilEnabled = false;
		//uint8 stencilReadMask; // stencilTestMask // pretty sure OpenGL does not support this.
		uint8 stencilWriteMask;
		StencilOpListDesc stencilFrontFaceCase;
		StencilOpListDesc stencilBackFaceCase;
	};

	// Triangle fans? Patch Lists?
	enum class PrimitiveTopology
	{
		Triangles,
		TriangleStrips,
		Points,
		Lines,
		LineStrips,
	};

	// TODO: move to pipelines since they're much more modern and hold up better with api's like vulkan and metal, also easier to performantly implement for me ;)
	struct PipelineStateDesc
	{
		const Shader* shader;
		//const ComputeShader* computeShader;

		const BlendStateDesc* blendState;
		const RasterizerStateDesc* rasterizerState;
		const DepthStencilStateDesc* dsState;

		const VertexBufferLayout* inputLayout;

		PrimitiveTopology primitiveType; // = PrimitiveTopology::Triangles;
		// Sample Mask?
	};

	struct PipelineState
	{
		PipelineStateDesc descriptor;
		CTL::Ref<void> internal;
	};
}