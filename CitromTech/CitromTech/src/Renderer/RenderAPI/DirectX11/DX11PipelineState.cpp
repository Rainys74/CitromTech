#include "DX11Device.h"
#include "Renderer/RenderAPI/PipelineState.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11DebugHandler.h"

#include "Math/MathCommon.h"
#include "Logger/Logger.h"

namespace Citrom::RenderAPI
{
	PipelineState DX11Device::CreatePipelineState(PipelineStateDesc* descriptor)
	{
		return PipelineState();
	}
	void DX11Device::BindPipelineState(PipelineState* ps)
	{

	}

	D3D11_BLEND DX11Device::BlendFactorToD3D11Blend(BlendFactor factor)
	{
#define BLENDOPT_TOD3D11CASE(x, y) case (x): return (y); break
		switch (factor)
		{
		default: return D3D11_BLEND_ONE; break;

			BLENDOPT_TOD3D11CASE(BlendFactor::Zero, D3D11_BLEND_ZERO);
			BLENDOPT_TOD3D11CASE(BlendFactor::One, D3D11_BLEND_ONE);
			BLENDOPT_TOD3D11CASE(BlendFactor::SrcAlpha, D3D11_BLEND_SRC_ALPHA);
			BLENDOPT_TOD3D11CASE(BlendFactor::OneMinusSrcAlpha, D3D11_BLEND_INV_SRC_ALPHA);
		}
	}

	D3D11_BLEND_OP DX11Device::BlendOpToD3D11BlendOp(BlendOp blendOp)
	{
		switch (blendOp)
		{
		default: return D3D11_BLEND_OP_ADD; break;

			BLENDOPT_TOD3D11CASE(BlendOp::Add, D3D11_BLEND_OP_ADD);
			BLENDOPT_TOD3D11CASE(BlendOp::Subtract, D3D11_BLEND_OP_SUBTRACT);
			BLENDOPT_TOD3D11CASE(BlendOp::ReverseSubtract, D3D11_BLEND_OP_REV_SUBTRACT);
			BLENDOPT_TOD3D11CASE(BlendOp::Min, D3D11_BLEND_OP_MIN);
			BLENDOPT_TOD3D11CASE(BlendOp::Max, D3D11_BLEND_OP_MAX);
		}
	}

	UINT DX11Device::RenderTargetWriteMaskToD3D11(RenderTargetWriteMask mask)
	{
#define MASK_MATCH(MASK1, MASK2, DO) if (static_cast<uint8fast>(MASK1) & static_cast<uint8fast>(MASK2)) {DO;}

		//if (mask & RenderTargetWriteMask::Red) writeMask |= D3D11_COLOR_WRITE_ENABLE_RED;
		//if (mask & RenderTargetWriteMask::Green) writeMask |= D3D11_COLOR_WRITE_ENABLE_GREEN;
		//if (mask & RenderTargetWriteMask::Blue) writeMask |= D3D11_COLOR_WRITE_ENABLE_BLUE;
		//if (mask & RenderTargetWriteMask::Alpha) writeMask |= D3D11_COLOR_WRITE_ENABLE_ALPHA;

		UINT writeMask = 0;
		MASK_MATCH(mask, RenderTargetWriteMask::Red, writeMask |= D3D11_COLOR_WRITE_ENABLE_RED);
		MASK_MATCH(mask, RenderTargetWriteMask::Green, writeMask |= D3D11_COLOR_WRITE_ENABLE_GREEN);
		MASK_MATCH(mask, RenderTargetWriteMask::Blue, writeMask |= D3D11_COLOR_WRITE_ENABLE_BLUE);
		MASK_MATCH(mask, RenderTargetWriteMask::Alpha, writeMask |= D3D11_COLOR_WRITE_ENABLE_ALPHA);
		return writeMask;
	}

	D3D11_FILL_MODE DX11Device::FillModeToD3D11FillMode(FillMode fillMode)
	{
#define RASTOPT_TOD3D11CASE(x, y) case (x): return (y); break
		switch (fillMode)
		{
		default: return D3D11_FILL_SOLID; break;

			RASTOPT_TOD3D11CASE(FillMode::Solid, D3D11_FILL_SOLID);
			RASTOPT_TOD3D11CASE(FillMode::Wireframe, D3D11_FILL_WIREFRAME);
		}
	}
	D3D11_CULL_MODE DX11Device::CullModeToD3D11CullMode(CullMode cullMode)
	{
		switch (cullMode)
		{
		default: return D3D11_CULL_BACK; break;

			RASTOPT_TOD3D11CASE(CullMode::Back, D3D11_CULL_BACK);
			RASTOPT_TOD3D11CASE(CullMode::Front, D3D11_CULL_FRONT);
		}
	}
}
#endif