#include "DX11Device.h"
#include "Renderer/RenderAPI/PipelineState.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11DebugHandler.h"

#include "Math/MathCommon.h"
#include "Logger/Logger.h"

namespace Citrom::RenderAPI
{
	struct PipelineStateDX11
	{
		WRL::ComPtr<ID3D11BlendState> blendState;
		WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
		WRL::ComPtr<ID3D11DepthStencilState> dsState;
	};

	PipelineState DX11Device::CreatePipelineState(PipelineStateDesc* descriptor)
	{
		CREATE_BUFFER_INTERNAL(PipelineState, PipelineStateDX11, pipeline, internalData);

		const auto* blendSpec = descriptor->blendState;
		const auto* rasterDesc = descriptor->rasterizerState;
		const auto* dsDesc = descriptor->dsState;

		HRESULT hr;

		if (blendSpec)
		{
			D3D11_BLEND_DESC bd = {};
			bd.RenderTarget[0].BlendEnable = true;
			bd.RenderTarget[0].SrcBlend = BlendFactorToD3D11Blend(blendSpec->srcBlend);
			bd.RenderTarget[0].DestBlend = BlendFactorToD3D11Blend(blendSpec->destBlend);
			bd.RenderTarget[0].BlendOp = BlendOpToD3D11BlendOp(blendSpec->blendOperation);
			bd.RenderTarget[0].SrcBlendAlpha = BlendFactorToD3D11Blend(blendSpec->srcBlendAlpha); // TODO: figure these alpha males out and their differences from beta
			bd.RenderTarget[0].DestBlendAlpha = BlendFactorToD3D11Blend(blendSpec->destBlendAlpha);
			bd.RenderTarget[0].BlendOpAlpha = BlendOpToD3D11BlendOp(blendSpec->blendOperationAlpha);
			bd.RenderTarget[0].RenderTargetWriteMask = RenderTargetWriteMaskToD3D11(blendSpec->renderTargetWriteMask);

			DXCallHR(m_Device->CreateBlendState(&bd, &internalData->blendState));

			//DXCall(m_DeviceContext->OMSetBlendState(internalData->blendState.Get(), nullptr, 0xFFFFFFFF));
		}
		if (rasterDesc)
		{
			D3D11_RASTERIZER_DESC rd = {};
			rd.FillMode = FillModeToD3D11FillMode(rasterDesc->fillMode);
			rd.CullMode = CullModeToD3D11CullMode(rasterDesc->cullMode);
			rd.FrontCounterClockwise = rasterDesc->frontCounterClockwise;

			DXCallHR(m_Device->CreateRasterizerState(&rd, &internalData->rasterizerState));

			//DXCall(m_DeviceContext->RSSetState(internalData->rasterizerState.Get()));
		}
		if (dsDesc)
		{
			D3D11_DEPTH_STENCIL_DESC dsd = {};
			dsd.DepthEnable = dsDesc->depthEnabled;
			dsd.DepthWriteMask = dsDesc->depthWriteEnabled ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
			dsd.DepthFunc = DepthStencilComparisonFuncToD3D11(dsDesc->depthFunc);
			if (dsDesc->stencilEnabled)
			{
				dsd.StencilEnable = dsDesc->stencilEnabled;
				dsd.StencilReadMask = 0xFF;
				dsd.StencilWriteMask = dsDesc->stencilWriteMask;

				dsd.FrontFace.StencilFailOp = StencilOpToD3D11(dsDesc->stencilFrontFaceCase.stencilDepthFailOp);
				dsd.FrontFace.StencilDepthFailOp = StencilOpToD3D11(dsDesc->stencilFrontFaceCase.stencilPassDepthFailOp);
				dsd.FrontFace.StencilPassOp = StencilOpToD3D11(dsDesc->stencilFrontFaceCase.stencilDepthPassOp);
				dsd.FrontFace.StencilFunc = DepthStencilComparisonFuncToD3D11(dsDesc->stencilFrontFaceCase.stencilFunc);

				dsd.BackFace.StencilFunc = DepthStencilComparisonFuncToD3D11(dsDesc->stencilBackFaceCase.stencilFunc);
				dsd.BackFace.StencilFailOp = StencilOpToD3D11(dsDesc->stencilBackFaceCase.stencilDepthFailOp);
				dsd.BackFace.StencilDepthFailOp = StencilOpToD3D11(dsDesc->stencilBackFaceCase.stencilPassDepthFailOp);
				dsd.BackFace.StencilPassOp = StencilOpToD3D11(dsDesc->stencilBackFaceCase.stencilDepthPassOp);
			}

			DXCallHR(m_Device->CreateDepthStencilState(&dsd, &internalData->dsState));

			//DXCall(m_DeviceContext->OMSetDepthStencilState(internalData->dsState.Get(), 1u));
		}

		DXCall(m_DeviceContext->IASetPrimitiveTopology(PrimitiveTopologyToD3D(descriptor->primitiveType)));

		return pipeline;
	}
	void DX11Device::RCBindPipelineState(PipelineState* ps, CommandBuffer* cmd)
	{
		GET_BUFFER_INTERNAL(PipelineStateDX11, ps, internalData);

		// TODO: do if checks slow down the program? Add if checks to not try to bind invalid stuff
		DXCall(m_DeviceContext->OMSetBlendState(internalData->blendState.Get(), nullptr, 0xFFFFFFFF));
		DXCall(m_DeviceContext->RSSetState(internalData->rasterizerState.Get()));
		DXCall(m_DeviceContext->OMSetDepthStencilState(internalData->dsState.Get(), 1u));

		DXCall(m_DeviceContext->IASetPrimitiveTopology(PrimitiveTopologyToD3D(ps->descriptor.primitiveType)));

		BindShader(const_cast<Shader*>(ps->descriptor.shader));
		BindVertexBufferLayout(const_cast<VertexBufferLayout*>(ps->descriptor.inputLayout));
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
			RASTOPT_TOD3D11CASE(CullMode::None, D3D11_CULL_NONE);
		}
	}

	D3D_PRIMITIVE_TOPOLOGY DX11Device::PrimitiveTopologyToD3D(PrimitiveTopology primitives)
	{
#define PRIMOPT_TOD3DCASE(x, y) RASTOPT_TOD3D11CASE(x, y)
		switch (primitives)
		{
			default: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;

			PRIMOPT_TOD3DCASE(PrimitiveTopology::Triangles, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			PRIMOPT_TOD3DCASE(PrimitiveTopology::TriangleStrips, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			PRIMOPT_TOD3DCASE(PrimitiveTopology::Points, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			PRIMOPT_TOD3DCASE(PrimitiveTopology::Lines, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			PRIMOPT_TOD3DCASE(PrimitiveTopology::LineStrips, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		}
		return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
	}

	D3D11_COMPARISON_FUNC DX11Device::DepthStencilComparisonFuncToD3D11(DepthStencilComparisonFunc func)
	{
#define COMPFUNC_TOD3D11CASE(x, y) PRIMOPT_TOD3DCASE(x, y)
		switch (func)
		{
			default: return D3D11_COMPARISON_LESS; break;

			COMPFUNC_TOD3D11CASE(DepthStencilComparisonFunc::Never, D3D11_COMPARISON_NEVER);
			COMPFUNC_TOD3D11CASE(DepthStencilComparisonFunc::Equal, D3D11_COMPARISON_EQUAL);
			COMPFUNC_TOD3D11CASE(DepthStencilComparisonFunc::NotEqual, D3D11_COMPARISON_NOT_EQUAL);
			COMPFUNC_TOD3D11CASE(DepthStencilComparisonFunc::Less, D3D11_COMPARISON_LESS);
			COMPFUNC_TOD3D11CASE(DepthStencilComparisonFunc::LessEqual, D3D11_COMPARISON_LESS_EQUAL);
			COMPFUNC_TOD3D11CASE(DepthStencilComparisonFunc::Greater, D3D11_COMPARISON_GREATER);
			COMPFUNC_TOD3D11CASE(DepthStencilComparisonFunc::GreaterEqual, D3D11_COMPARISON_GREATER_EQUAL);
			COMPFUNC_TOD3D11CASE(DepthStencilComparisonFunc::Always, D3D11_COMPARISON_ALWAYS);
		}
	}

	D3D11_STENCIL_OP DX11Device::StencilOpToD3D11(StencilOp op)
	{
#define STENCILOP_TOD3D11CASE(x, y) COMPFUNC_TOD3D11CASE(x, y)
		switch (op)
		{
			default: return D3D11_STENCIL_OP_KEEP; break;

			STENCILOP_TOD3D11CASE(StencilOp::Keep, D3D11_STENCIL_OP_KEEP);
			STENCILOP_TOD3D11CASE(StencilOp::Zero, D3D11_STENCIL_OP_ZERO);
			STENCILOP_TOD3D11CASE(StencilOp::Replace, D3D11_STENCIL_OP_REPLACE);
			STENCILOP_TOD3D11CASE(StencilOp::IncrementAndClamp, D3D11_STENCIL_OP_INCR_SAT);
			STENCILOP_TOD3D11CASE(StencilOp::DecrementAndClamp, D3D11_STENCIL_OP_DECR_SAT);
			STENCILOP_TOD3D11CASE(StencilOp::Invert, D3D11_STENCIL_OP_INVERT);
			STENCILOP_TOD3D11CASE(StencilOp::IncrementAndWrap, D3D11_STENCIL_OP_INCR);
			STENCILOP_TOD3D11CASE(StencilOp::DecrementAndWrap, D3D11_STENCIL_OP_DECR);
		}
	}
}
#endif
