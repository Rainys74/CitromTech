#include "DX11Device.h"
#include "Renderer/RenderAPI/Buffer.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11DebugHandler.h"

#include "Math/MathCommon.h"
#include "Logger/Logger.h"

namespace Citrom::RenderAPI
{
	void DX11Device::CreateRenderTarget()
	{
		HRESULT hr;

		// Gain access to texture subresource in swap chain (back buffer)
		WRL::ComPtr<ID3D11Resource> backBuffer;
		DXCallHR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer)); // or IID_PPV_ARGS(&backBuffer)

		DXCallHR(m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_RenderTarget));

		// Depth Stencil View
		{
			D3D11_DEPTH_STENCIL_DESC dsd = {};
			dsd.DepthEnable = true;
			dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			dsd.DepthFunc = D3D11_COMPARISON_LESS;

			WRL::ComPtr<ID3D11DepthStencilState> dsState;
			DXCallHR(m_Device->CreateDepthStencilState(&dsd, &dsState));

			DXCall(m_DeviceContext->OMSetDepthStencilState(dsState.Get(), 1u));

			// Depth Stencil Texture
			WRL::ComPtr<ID3D11Texture2D> depthStencilTex;
			D3D11_TEXTURE2D_DESC td = {};
			td.Width = m_Width;
			td.Height = m_Height;
			td.MipLevels = 1;
			td.ArraySize = 1;
			td.Format = DXGI_FORMAT_D32_FLOAT; //DXGI_FORMAT_D24_UNORM_S8_UINT or DXGI_FORMAT_D32_FLOAT
			td.SampleDesc.Count = 1;
			td.SampleDesc.Quality = 0;
			td.Usage = D3D11_USAGE_DEFAULT;
			td.BindFlags = D3D11_BIND_DEPTH_STENCIL;

			DXCallHR(m_Device->CreateTexture2D(&td, nullptr, &depthStencilTex));

			// Create DSV
			D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
			dsvd.Format = DXGI_FORMAT_D32_FLOAT; // or use DXGI_FORMAT_UNKNOWN to use the texture's
			dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsvd.Texture2D.MipSlice = 0;

			DXCallHR(m_Device->CreateDepthStencilView(depthStencilTex.Get(), &dsvd, &m_DepthStencilView));
		}

		DXCall(m_DeviceContext->OMSetRenderTargets(1, &m_RenderTarget, m_DepthStencilView.Get()));
	}
	void DX11Device::DestroyRenderTarget()
	{
		if (m_RenderTarget)
		{
			m_RenderTarget->Release();
			m_RenderTarget = nullptr;
		}
	}

	void DX11Device::MakeSwapChain(SwapChainDesc* descriptor, BlendStateDesc* blendSpec)
	{
		m_Width = descriptor->windowPtr->GetBackend()->GetWidth();
		m_Height = descriptor->windowPtr->GetBackend()->GetHeight();

		DXGI_SWAP_CHAIN_DESC scd = {};
		scd.BufferDesc.Width = 0;
		scd.BufferDesc.Height = 0;
		scd.BufferDesc.Format = FormatToDXGIFormat(descriptor->renderFormat);
		scd.BufferDesc.RefreshRate.Numerator = 0;
		scd.BufferDesc.RefreshRate.Denominator = 0;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;
		scd.OutputWindow = (HWND)descriptor->windowPtr->Win32TryGetHWnd();
		scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = 0x00000000;

		HRESULT hr;

		DXCallHR(CreateDXGIFactory(IID_PPV_ARGS(&m_DXGIFactory)));

		DXCallHR(m_DXGIFactory->CreateSwapChain(m_Device, &scd, &m_SwapChain));

		CreateRenderTarget();
		Resize(m_Width, m_Height); // Create viewport to not depend on Win32's resize call on UpdateWindow

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

			WRL::ComPtr<ID3D11BlendState> blendState; // TODO: do i need to store this?

			DXCallHR(m_Device->CreateBlendState(&bd, &blendState));

			DXCall(m_DeviceContext->OMSetBlendState(blendState.Get(), nullptr, 0xFFFFFFFF));
		}
	}
	void DX11Device::SwapBuffers()
	{
		HRESULT hr;
		if (FAILED(hr = m_SwapChain->Present(m_VSyncInterval, 0x00000000)))
		{
			if (hr == DXGI_ERROR_DEVICE_REMOVED)
			{
				hr = m_Device->GetDeviceRemovedReason();
			}
			else
			{
				hr = hr;
			}
		}
		DXCallHR(hr);
	}
	void DX11Device::SetVSync(VSyncMode vSync)
	{
		UINT swapInterval = static_cast<UINT>(vSync);

		if (!IS_BETWEEN(swapInterval, 0, 4))
		{
			CT_CORE_WARN("Invalid VSync interval: {}. Setting to default (1).", swapInterval);
			swapInterval = 1;
		}

		m_VSyncInterval = swapInterval;
	}
	VSyncMode DX11Device::GetVSync()
	{
		return static_cast<VSyncMode>(m_VSyncInterval);
	}

	void DX11Device::Resize(uint32 width, uint32 height)
	{
		m_Width = width;
		m_Height = height;

		DestroyRenderTarget();

		HRESULT hr;
		DXCallHR(m_SwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0));

		CreateRenderTarget();

		// Configure viewport TODO: maybe separate this into ResizeViewport?
		D3D11_VIEWPORT vp;
		vp.Width = width;
		vp.Height = height;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0; // xPos
		vp.TopLeftY = 0; // yPos
		DXCall(m_DeviceContext->RSSetViewports(1, &vp));
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
}
#endif