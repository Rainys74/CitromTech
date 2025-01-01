#include "DX11Device.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11Includes.h"
#include "DX11DebugHandler.h"

extern "C"
{
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

namespace Citrom::RenderAPI
{
	DX11Device::DX11Device()
	{
		//DXGI_SWAP_CHAIN_DESC scd = {};
		//scd.BufferDesc.Width = 0;
		//scd.BufferDesc.Height = 0;
		//scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		//scd.BufferDesc.RefreshRate.Numerator = 0;
		//scd.BufferDesc.RefreshRate.Denominator = 0;
		//scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		//scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		//scd.SampleDesc.Count = 1;
		//scd.SampleDesc.Quality = 0;
		//scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		//scd.BufferCount = 1;
		//scd.OutputWindow = (HWND)window->Win32TryGetHWND();
		//scd.Windowed = TRUE;
		//scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		//scd.Flags = 0x00000000;
		//
		//HRESULT hr;
		//
		//// create device and front/back buffers, and swap chain and rendering context
		//DXCallHR(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE /*D3D_DRIVER_TYPE_UNKNOWN*/, nullptr, D3D11_CREATE_DEVICE_DEBUG /*swapflagslvalue |= D3D11_CREATE_DEVICE_DEBUG*/, nullptr, 0, D3D11_SDK_VERSION, &scd, &m_SwapChain, &m_Device, nullptr, &m_DeviceContext));
		//
		//// gain access to texture subresource in swap chain (back buffer)
		//WRL::ComPtr<ID3D11Resource> backBuffer;
		//m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
		//
		//m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_RenderTarget);

		/*
		 HRESULT D3D11CreateDevice(
								  [in, optional]  IDXGIAdapter            *pAdapter,
												  D3D_DRIVER_TYPE         DriverType,
												  HMODULE                 Software,
												  UINT                    Flags,
								  [in, optional]  const D3D_FEATURE_LEVEL *pFeatureLevels,
												  UINT                    FeatureLevels,
												  UINT                    SDKVersion,
								  [out, optional] ID3D11Device            **ppDevice,
								  [out, optional] D3D_FEATURE_LEVEL       *pFeatureLevel,
								  [out, optional] ID3D11DeviceContext     **ppImmediateContext
								);

		HRESULT D3D11CreateDeviceAndSwapChain(
								  [in, optional]  IDXGIAdapter               *pAdapter,
												  D3D_DRIVER_TYPE            DriverType,
												  HMODULE                    Software,
												  UINT                       Flags,
								  [in, optional]  const D3D_FEATURE_LEVEL    *pFeatureLevels,
												  UINT                       FeatureLevels,
												  UINT                       SDKVersion,
								  [in, optional]  const DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,
								  [out, optional] IDXGISwapChain             **ppSwapChain,
								  [out, optional] ID3D11Device               **ppDevice,
								  [out, optional] D3D_FEATURE_LEVEL          *pFeatureLevel,
								  [out, optional] ID3D11DeviceContext        **ppImmediateContext
								);
		*/
		//D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE /*D3D_DRIVER_TYPE_UNKNOWN*/, nullptr, D3D11_CREATE_DEVICE_DEBUG /*swapflagslvalue |= D3D11_CREATE_DEVICE_DEBUG*/, nullptr, 0, D3D11_SDK_VERSION, &scd, &m_SwapChain, &m_Device, nullptr, &m_DeviceContext);
		//D3D11CreateDevice			   (nullptr, D3D_DRIVER_TYPE_HARDWARE /*D3D_DRIVER_TYPE_UNKNOWN*/, nullptr, D3D11_CREATE_DEVICE_DEBUG /*swapflagslvalue |= D3D11_CREATE_DEVICE_DEBUG*/, nullptr, 0, D3D11_SDK_VERSION,					   &m_Device, nullptr, &m_DeviceContext);

		HRESULT hr;

		DXCallHR(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE /*D3D_DRIVER_TYPE_UNKNOWN*/, nullptr, D3D11_CREATE_DEVICE_DEBUG /*swapflagslvalue |= D3D11_CREATE_DEVICE_DEBUG*/, nullptr, 0, D3D11_SDK_VERSION, &m_Device, nullptr, &m_DeviceContext));
	}
	DX11Device::~DX11Device()
	{
	}
}
#endif