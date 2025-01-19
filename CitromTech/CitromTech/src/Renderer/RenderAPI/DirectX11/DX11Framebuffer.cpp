#include "DX11Device.h"
#include "Renderer/RenderAPI/Framebuffer.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11DebugHandler.h"

namespace Citrom::RenderAPI
{
	struct FramebufferDX11
	{
		WRL::ComPtr<ID3D11RenderTargetView> renderTarget;
		WRL::ComPtr<ID3D11Texture2D> renderTexture;
		WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	};

	Framebuffer DX11Device::CreateFramebuffer(FramebufferDesc* descriptor)
	{
		Framebuffer fb;
		fb.internal = CTL::CreateRef<FramebufferDX11>();
		auto internalData = static_cast<FramebufferDX11*>(fb.internal.get());
		fb.descriptor = *descriptor;

		// Create Render Target
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = descriptor->width;
		textureDesc.Height = descriptor->height;
		textureDesc.MipLevels = 1; // Usually 1 for render targets
		textureDesc.ArraySize = 1; // Single texture
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // DXGI_FORMAT_R8G8B8A8_UNORM // Use your desired DXGI_FORMAT
		textureDesc.SampleDesc.Count = descriptor->samples; // Multisampling count
		textureDesc.SampleDesc.Quality = 0; // Multisampling quality
		textureDesc.Usage = D3D11_USAGE_DEFAULT; // Default usage
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; // Bind as render target and shader resource
		textureDesc.CPUAccessFlags = 0; // No CPU access needed
		textureDesc.MiscFlags = 0; // No additional flags

		HRESULT hr;
		DXCallHR(m_Device->CreateTexture2D(&textureDesc, nullptr, &internalData->renderTexture));

		DXCallHR(m_Device->CreateRenderTargetView(internalData->renderTexture.Get(), nullptr, &internalData->renderTarget));

		// Create Depth Stencil
		D3D11_DEPTH_STENCIL_DESC dsd = {};
		dsd.DepthEnable = true;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsd.DepthFunc = D3D11_COMPARISON_LESS;

		WRL::ComPtr<ID3D11DepthStencilState> dsState;
		DXCallHR(m_Device->CreateDepthStencilState(&dsd, &dsState));

		//DXCall(m_DeviceContext->OMSetDepthStencilState(dsState.Get(), 1u));

		// Depth Stencil Texture
		WRL::ComPtr<ID3D11Texture2D> depthStencilTex;
		D3D11_TEXTURE2D_DESC td = {};
		td.Width = descriptor->width;
		td.Height = descriptor->height;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_D32_FLOAT;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		DXCallHR(m_Device->CreateTexture2D(&td, nullptr, &depthStencilTex));

		// Create DS View
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
		dsvd.Format = DXGI_FORMAT_D32_FLOAT; // or use DXGI_FORMAT_UNKNOWN to use the texture's
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvd.Texture2D.MipSlice = 0;

		DXCallHR(m_Device->CreateDepthStencilView(depthStencilTex.Get(), &dsvd, &internalData->depthStencilView));

		return fb;
	}
	void DX11Device::SetTargetFramebuffer(Framebuffer* fb)
	{
		if (fb == nullptr)
		{
			//DestroyRenderTarget();
			//CreateRenderTarget();
			DXCall(m_DeviceContext->OMSetRenderTargets(1, &m_RenderTarget, nullptr));
		}
		else
		{
			auto internalData = static_cast<FramebufferDX11*>(fb->internal.get());

			//DXCall(m_DeviceContext->OMSetRenderTargets(1, &internalData->renderTarget, internalData->depthStencilView.Get()));
			ID3D11RenderTargetView* renderTargets[2] = { internalData->renderTarget.Get(), m_RenderTarget};
			DXCall(m_DeviceContext->OMSetRenderTargets(1, renderTargets /*&internalData->renderTarget*/, nullptr));
		}
	}
	void* DX11Device::GetFramebufferColorAttachment(Framebuffer* fb)
	{
		auto internalData = static_cast<FramebufferDX11*>(fb->internal.get());

		return internalData->renderTexture.Get();
	}
}
#endif