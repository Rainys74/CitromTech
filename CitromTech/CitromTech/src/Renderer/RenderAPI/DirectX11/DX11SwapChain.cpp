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

		m_DeviceContext->OMSetRenderTargets(1, &m_RenderTarget, nullptr);
	}
	void DX11Device::DestroyRenderTarget()
	{
		if (m_RenderTarget)
		{
			m_RenderTarget->Release();
			m_RenderTarget = nullptr;
		}
	}

	void DX11Device::MakeSwapChain(SwapChainDesc* descriptor)
	{
		DXGI_SWAP_CHAIN_DESC scd = {};
		scd.BufferDesc.Width = 0;
		scd.BufferDesc.Height = 0;
		scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
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

	void DX11Device::Resize(uint32 width, uint32 height)
	{
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
		m_DeviceContext->RSSetViewports(1, &vp);
	}
}
#endif