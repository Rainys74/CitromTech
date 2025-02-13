#include "DX11Device.h"
#include "Renderer/RenderAPI/Shader.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11DebugHandler.h"
#include "CitromAssert.h"

namespace Citrom::RenderAPI
{
	struct ShaderDX11
	{
		WRL::ComPtr<ID3D11VertexShader> vertexShader;
		WRL::ComPtr<ID3D11PixelShader> pixelShader;

		//WRL::ComPtr<ID3DBlob> blob;
		WRL::ComPtr<ID3DBlob> vertexBlob;
		WRL::ComPtr<ID3DBlob> pixelBlob;
	};

	static std::wstring GetShaderFileName(const std::string& shaderName, ShaderType shaderFormat)
	{
		std::wstring wideFilePath(shaderName.begin(), shaderName.end());
		wideFilePath.insert(0, L"ShaderCache/");
		
		switch (shaderFormat)
		{
			case ShaderType::Vertex:
				wideFilePath.append(L"_vs");
				break;
			case ShaderType::Fragment:
				wideFilePath.append(L"_fs");
				break;
			default:
				CT_CORE_ASSERT(false, "Unsupported Shader Format!");
				break;
		}

		wideFilePath.append(L".dxbc");

		return wideFilePath;
	}

	Shader DX11Device::CreateShader(ShaderDesc* descriptor)
	{
		CREATE_BUFFER_INTERNAL(Shader, ShaderDX11, shader, internalData);

		HRESULT hr;
		
		// TODO: whenever you pass in a shader that doesn't exist: you get a: the system cannot find the specified file DX error, figure out how to handle this gracefully.
		// Vertex Shader
		DXCallHR(D3DReadFileToBlob(GetShaderFileName(descriptor->name, ShaderType::Vertex).c_str(), &internalData->vertexBlob));
		DXCallHR(m_Device->CreateVertexShader(internalData->vertexBlob->GetBufferPointer(), internalData->vertexBlob->GetBufferSize(), nullptr, &internalData->vertexShader));

		// Pixel Shader
		DXCallHR(D3DReadFileToBlob(GetShaderFileName(descriptor->name, ShaderType::Fragment).c_str(), &internalData->pixelBlob));
		DXCallHR(m_Device->CreatePixelShader(internalData->pixelBlob->GetBufferPointer(), internalData->pixelBlob->GetBufferSize(), nullptr, &internalData->pixelShader));

		return shader;
	}
	void DX11Device::BindShader(Shader* shader)
	{
		GET_BUFFER_INTERNAL(ShaderDX11, shader, internalData);

		// Bind Vertex Shader
		DXCall(m_DeviceContext->VSSetShader(internalData->vertexShader.Get(), 0, 0));

		// Bind Fragment Shader
		DXCall(m_DeviceContext->PSSetShader(internalData->pixelShader.Get(), 0, 0));
	}

	// TODO: how to handle setting shaders?
	void DX11Device::SetName(Shader* resource, const char* name)
	{
		GET_BUFFER_INTERNAL(ShaderDX11, resource, internalData);
	}

	WRL::ComPtr<ID3DBlob> DX11Device::DX11GetVertexShaderBlob(const Shader* shader)
	{
		auto internalData = static_cast<ShaderDX11*>(shader->internal.get());

		CT_CORE_ASSERT(shader, "Shader is null!");
		CT_CORE_ASSERT(internalData->vertexShader, "Vertex Shader is null!");
		CT_CORE_ASSERT(internalData->vertexBlob, "Vertex Shader Blob is null!");

		return internalData->vertexBlob;
	}

	DXGI_FORMAT DX11Device::FormatToDXGIFormat(Format format)
	{
#define FORMAT_TODXGICASE(x, y) case (x): return (y); break
		switch (format)
		{
			default: return DXGI_FORMAT_UNKNOWN; break;

			FORMAT_TODXGICASE(Format::Unknown, DXGI_FORMAT_UNKNOWN);
			FORMAT_TODXGICASE(Format::R32_FLOAT, DXGI_FORMAT_R32_FLOAT);
			FORMAT_TODXGICASE(Format::R32G32_FLOAT, DXGI_FORMAT_R32G32_FLOAT);
			FORMAT_TODXGICASE(Format::R32G32B32_FLOAT, DXGI_FORMAT_R32G32B32_FLOAT);
			FORMAT_TODXGICASE(Format::R32G32B32A32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT);

			FORMAT_TODXGICASE(Format::R8_U2FNORM, DXGI_FORMAT_R8_UNORM);
			FORMAT_TODXGICASE(Format::R8G8_U2FNORM, DXGI_FORMAT_R8G8_UNORM);
			FORMAT_TODXGICASE(Format::R8G8B8A8_U2FNORM, DXGI_FORMAT_R8G8B8A8_UNORM);
			FORMAT_TODXGICASE(Format::B8G8R8A8_U2FNORM, DXGI_FORMAT_B8G8R8A8_UNORM);
		}
	}
	DXGI_FORMAT DX11Device::FBFormatToDXGIFormat(FramebufferFormat format)
	{
		switch (format)
		{
			default: return DXGI_FORMAT_UNKNOWN; break;

			FORMAT_TODXGICASE(FramebufferFormat::RGBA8, DXGI_FORMAT_R8G8B8A8_UNORM);
			FORMAT_TODXGICASE(FramebufferFormat::DEPTH24STENCIL8, DXGI_FORMAT_D24_UNORM_S8_UINT);
			FORMAT_TODXGICASE(FramebufferFormat::D32F, DXGI_FORMAT_D32_FLOAT);
		}
	}

	D3D11_USAGE DX11Device::UsageToD3D11Usage(Usage usage)
	{
		switch (usage)
		{
			default:
			case Usage::Static:
				return D3D11_USAGE_DEFAULT;
				break;
			case Usage::Dynamic:
				return D3D11_USAGE_DYNAMIC;
				break;
		}
	}
}
#endif