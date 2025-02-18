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
		td.Height = descriptor->height; // size values of EACH texture
		td.MipLevels = descriptor->mipLevels;
		td.ArraySize = descriptor->arraySize;
		td.Format = FormatToDXGIFormat(descriptor->format);
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = UsageToD3D11Usage(descriptor->usage);
		td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		if (td.Usage == D3D11_USAGE_DYNAMIC)
			td.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
		td.MiscFlags = 0;

		//if (descriptor->mipLevels == 0)
		//	td.MipLevels = static_cast<UINT>(log2(max(descriptor->width, descriptor->height))) + 1;

		if (descriptor->mipLevels != 1)
		{
			td.BindFlags |= D3D11_BIND_RENDER_TARGET;
			td.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS; // should only happen for 0 mipmaps, i think.
		}

		D3D11_SUBRESOURCE_DATA tsd = {};
		tsd.pSysMem = descriptor->data;
		tsd.SysMemPitch = static_cast<UINT>(descriptor->width * GetFormatSize(descriptor->format));

		HRESULT hr;
		DXCallHR(m_Device->CreateTexture2D(&td, &tsd, &internalData->texture));

		// Create Texture View
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
		srvd.Format = td.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		if (descriptor->arraySize > 1)
		{
			srvd.Texture2DArray.ArraySize = descriptor->arraySize;
			srvd.Texture2DArray.MostDetailedMip = 0;
			srvd.Texture2DArray.MipLevels = descriptor->mipLevels;
			srvd.Texture2DArray.FirstArraySlice = 0;
		}
		else
		{
			srvd.Texture2D.MostDetailedMip = 0;
			srvd.Texture2D.MipLevels = descriptor->mipLevels;
		}

		DXCallHR(m_Device->CreateShaderResourceView(internalData->texture.Get(), &srvd, &internalData->textureView));
		//DXCall(m_DeviceContext->UpdateSubresource(internalData->texture.Get(), 0, 0, tsd.pSysMem, tsd.SysMemPitch, 0));
		if (descriptor->mipLevels != 1 && HAS_FLAG(td.MiscFlags, D3D11_RESOURCE_MISC_GENERATE_MIPS)) {
			DXCall(m_DeviceContext->GenerateMips(internalData->textureView.Get())); // 0 or -1?? will generate log2(std::max(pTexture->desc.Width, pTexture->desc.Height)) + 1
		}

		// Sampler
		D3D11_SAMPLER_DESC sd = {};
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // for translating 3 separate filters to this, create some sort of hash map that uses a struct of 3 agnostic filters to look-up d3d11 equivalent or something.
		sd.AddressU = TextureAddressModeToD3D11(descriptor->sampler.addressU);
		sd.AddressV = TextureAddressModeToD3D11(descriptor->sampler.addressV);
		sd.AddressW = TextureAddressModeToD3D11(descriptor->sampler.addressW);

		DXCallHR(m_Device->CreateSamplerState(&sd, &internalData->sampler));

		return tex2D;
	}
	void DX11Device::RCBindTexture2D(Texture2D* tex2D, uint32 startSlot, CommandBuffer* cmd)
	{
		GET_BUFFER_INTERNAL(Texture2DDX11, tex2D, internalData);

		DXCall(m_DeviceContext->PSSetShaderResources(startSlot, tex2D->descriptor.arraySize, internalData->textureView.GetAddressOf()));
		DXCall(m_DeviceContext->PSSetSamplers(startSlot, tex2D->descriptor.arraySize, internalData->sampler.GetAddressOf()));
	}

	// TODO: is this ok?
	void DX11Device::SetName(Texture2D* resource, const char* name)
	{
		GET_BUFFER_INTERNAL(Texture2DDX11, resource, internalData);
		TSetResourceNameDX11(internalData->texture.Get(), name);
		TSetResourceNameDX11(internalData->textureView.Get(), name);
		TSetResourceNameDX11(internalData->sampler.Get(), name);
	}

	D3D11_TEXTURE_ADDRESS_MODE DX11Device::TextureAddressModeToD3D11(TextureAddressMode mode)
	{
	#define TEXOPT_TOD3D11CASE(x, y) case (x): return (y); break
		switch (mode)
		{
			default: return D3D11_TEXTURE_ADDRESS_WRAP; break;
		}
	}
}
#endif
