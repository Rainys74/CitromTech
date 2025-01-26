#include "DX11Device.h"
#include "Renderer/RenderAPI/Texture.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11DebugHandler.h"

namespace Citrom::RenderAPI
{
	struct Texture2DDX11
	{
		WRL::ComPtr<ID3D11Texture2D> texture;
		WRL::ComPtr<ID3D11ShaderResourceView> textureView;

		WRL::ComPtr<ID3D11SamplerState> sampler;
	};

	Texture2D DX11Device::CreateTexture2D(Texture2DDesc* descriptor)
	{
		CREATE_BUFFER_INTERNAL(Texture2D, Texture2DDX11, tex2D, internalData);

		D3D11_TEXTURE2D_DESC td = {};
		td.Width = descriptor->width;
		td.Height = descriptor->height;
		td.MipLevels = descriptor->mipLevels;
		td.ArraySize = 1;
		td.Format = FormatToDXGIFormat(descriptor->format);
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = UsageToD3D11Usage(descriptor->usage);
		td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		if (td.Usage == D3D11_USAGE_DYNAMIC)
			td.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
		td.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA tsd = {};
		tsd.pSysMem = descriptor->data;
		tsd.SysMemPitch = static_cast<UINT>(descriptor->width * GetFormatSize(descriptor->format));

		HRESULT hr;
		DXCallHR(m_Device->CreateTexture2D(&td, &tsd, &internalData->texture));

		// Create Texture View
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
		srvd.Format = td.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MostDetailedMip = 0;
		srvd.Texture2D.MipLevels = descriptor->mipLevels;

		DXCallHR(m_Device->CreateShaderResourceView(internalData->texture.Get(), &srvd, &internalData->textureView));

		// Sampler
		D3D11_SAMPLER_DESC sd = {};
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		DXCallHR(m_Device->CreateSamplerState(&sd, &internalData->sampler));

		return tex2D;
	}
	void DX11Device::BindTexture2D(Texture2D* tex2D)
	{
		GET_BUFFER_INTERNAL(Texture2DDX11, tex2D, internalData);

		DXCall(m_DeviceContext->PSSetShaderResources(0, 1, internalData->textureView.GetAddressOf()));
		DXCall(m_DeviceContext->PSSetSamplers(0, 1, internalData->sampler.GetAddressOf()));
	}
}
#endif