#include "DX11Device.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11Includes.h"
#include "DX11DebugHandler.h"

#include <codecvt>

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

#define _INTERN_DEVICE_FLAGS_DEBUG (D3D11_CREATE_DEVICE_DEBUG) // | D3D11_CREATE_DEVICE_DEBUGGABLE // "This value is not supported until Direct3D 11.1."
#define _INTERN_DEVICE_FLAGS_ALL (D3D11_CREATE_DEVICE_SINGLETHREADED) // TODO: this may cause issues someday, though improves performance at the cost of thread safety. // D3D11_CREATE_DEVICE_BGRA_SUPPORT

#ifdef CT_DEBUG
#define DEVICE_FLAGS (_INTERN_DEVICE_FLAGS_ALL | _INTERN_DEVICE_FLAGS_DEBUG) // D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS
#else
#define DEVICE_FLAGS _INTERN_DEVICE_FLAGS_ALL
#endif

		//inline UINT deviceFlags = 0x00000000;
		DXCallHR(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE /*D3D_DRIVER_TYPE_UNKNOWN*/, nullptr, DEVICE_FLAGS /*swapflagslvalue |= D3D11_CREATE_DEVICE_DEBUG*/, nullptr, 0, D3D11_SDK_VERSION, &m_Device, &m_D3DFeatureLevel, &m_DeviceContext));
	}
	DX11Device::~DX11Device()
	{
		m_Device->Release();
		m_DeviceContext->Release();
	}

	void DX11Device::WaitForGPU()
	{
		DXCall(m_DeviceContext->Flush());

		D3D11_QUERY_DESC qd;
		qd.MiscFlags = 0;
		qd.Query = D3D11_QUERY_EVENT;

		HRESULT hr;
		WRL::ComPtr<ID3D11Query> query;
		DXCallHR(m_Device->CreateQuery(&qd, &query));
		DXCall(m_DeviceContext->End(query.Get()));
		BOOL result;
		while (m_DeviceContext->GetData(query.Get(), &result, sizeof(result), 0) == S_FALSE); // TODO: sleep for 1 ms to avoid aggressive polling (should i? in a 100 fps case, a max amount of 1 ms overhead causes the fps to drop to 90.91..)
		//CT_CORE_ASSERT(result == TRUE);
	}

	GPUInfo DX11Device::GetCurrentGPUInfo()
	{
		HRESULT hr;

		WRL::ComPtr<IDXGIDevice> dxgiDevice;
		DXCallHR(m_Device->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));

		WRL::ComPtr<IDXGIAdapter> adapter;
		DXCallHR(dxgiDevice->GetAdapter(&adapter));

		// Retrieve the data from the adapter
		DXGI_ADAPTER_DESC adapterDesc;
		DXCallHR(adapter->GetDesc(&adapterDesc));

		// Fill up the struct
		GPUInfo gpuInfo;

		switch (adapterDesc.VendorId)
		{
		case 0x1002: // AMD
			gpuInfo.vendor = "AMD";
			break;
		case 0x10DE: // NVIDIA
			gpuInfo.vendor = "NVIDIA";
			break;
		case 0x8086: // Intel
			gpuInfo.vendor = "Intel";
			break;
		default:
			gpuInfo.vendor = "Unknown";
			break;
		}

		gpuInfo.renderer = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(adapterDesc.Description);

		switch (m_D3DFeatureLevel)
		{
			case D3D_FEATURE_LEVEL_12_2: gpuInfo.version = "Direct3D Feature Level: 12.2"; break;
			case D3D_FEATURE_LEVEL_12_1: gpuInfo.version = "Direct3D Feature Level: 12.1"; break;
			case D3D_FEATURE_LEVEL_12_0: gpuInfo.version = "Direct3D Feature Level: 12.0"; break;
			case D3D_FEATURE_LEVEL_11_1: gpuInfo.version = "Direct3D Feature Level: 11.1"; break;
			case D3D_FEATURE_LEVEL_11_0: gpuInfo.version = "Direct3D Feature Level: 11.0"; break;
			case D3D_FEATURE_LEVEL_10_1: gpuInfo.version = "Direct3D Feature Level: 10.1"; break;
			case D3D_FEATURE_LEVEL_10_0: gpuInfo.version = "Direct3D Feature Level: 10.0"; break;
			case D3D_FEATURE_LEVEL_9_3: gpuInfo.version =  "Direct3D Feature Level: 9.3"; break;
			case D3D_FEATURE_LEVEL_9_2: gpuInfo.version =  "Direct3D Feature Level: 9.2"; break;
			case D3D_FEATURE_LEVEL_9_1: gpuInfo.version =  "Direct3D Feature Level: 9.1"; break;
			case D3D_FEATURE_LEVEL_1_0_CORE: gpuInfo.version = "Direct3D Feature Level: 1.0 Core"; break;

			default: gpuInfo.version = "Unknown"; break;
		}
		/*
		switch (m_D3DFeatureLevel)
		{
			case D3D_FEATURE_LEVEL_12_2:
				gpuInfo.shadingLanguageVersion = "Shader Model 6.5";
			case D3D_FEATURE_LEVEL_12_1:
			case D3D_FEATURE_LEVEL_12_0:
			case D3D_FEATURE_LEVEL_11_1:
			case D3D_FEATURE_LEVEL_11_0:
				gpuInfo.shadingLanguageVersion = "Shader Model 5.1"; // DirectX 11 and Shader Model 5.0
				break;
		}
		*/
		switch (m_D3DFeatureLevel)
		{
			case D3D_FEATURE_LEVEL_12_2:
				gpuInfo.shadingLanguageVersion = "Shader Model 6.5";
			case D3D_FEATURE_LEVEL_12_1:
			case D3D_FEATURE_LEVEL_12_0:
				gpuInfo.shadingLanguageVersion = "Shader Model 5.1"; // DirectX 12 and Shader Model 6.x
				break;
			case D3D_FEATURE_LEVEL_11_1:
			case D3D_FEATURE_LEVEL_11_0:
				gpuInfo.shadingLanguageVersion = "Shader Model 5.0"; // DirectX 11 and Shader Model 5.0
				break;
			case D3D_FEATURE_LEVEL_10_1:
				gpuInfo.shadingLanguageVersion = "Shader Model 4.x"; // DirectX 10.1 and Shader Model 4.1
				break;
			case D3D_FEATURE_LEVEL_10_0:
				gpuInfo.shadingLanguageVersion = "Shader Model 4.0"; // DirectX 10 and Shader Model 4.0
				break;
			case D3D_FEATURE_LEVEL_9_3:
			case D3D_FEATURE_LEVEL_9_2:
			case D3D_FEATURE_LEVEL_9_1:
				gpuInfo.shadingLanguageVersion = "Shader Model 2.0"; // DirectX 9 and Shader Model 3.0
				break;
			case D3D_FEATURE_LEVEL_1_0_CORE:
				gpuInfo.shadingLanguageVersion = "Shader Model 2.0"; // DirectX 1.0 Core and Shader Model 2.0
				break;
			default:
				gpuInfo.shadingLanguageVersion = "Unknown Shader Model"; // In case of an unknown feature level
				break;
		}

		gpuInfo.videoMemory = adapterDesc.DedicatedVideoMemory;

		return gpuInfo;
	}
}
#endif