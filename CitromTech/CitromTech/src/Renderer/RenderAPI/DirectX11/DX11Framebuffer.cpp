#include "DX11Device.h"
#include "Renderer/RenderAPI/Framebuffer.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11DebugHandler.h"

#include <algorithm>

namespace Citrom::RenderAPI
{
	struct FramebufferDX11
	{
		CTL::DArray<FramebufferAttachment> colorAttachments;

		CTL::DArray<WRL::ComPtr<ID3D11RenderTargetView>> renderTargets;
		CTL::DArray<WRL::ComPtr<ID3D11Texture2D>> renderTextures;

		bool hasDepthStencil;

		WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
		WRL::ComPtr<ID3D11Texture2D> depthStencilTex;
	};

	Framebuffer DX11Device::CreateFramebuffer(FramebufferDesc* descriptor)
	{
		CREATE_BUFFER_INTERNAL(Framebuffer, FramebufferDX11, fb, internalData);

		const auto texWidth = descriptor->width == 0 ? m_Width : descriptor->width;
		const auto texHeight = descriptor->height == 0 ? m_Height : descriptor->height;

		for (auto& attachment : descriptor->attachments->attachments)
		{
			if (attachment.type == FramebufferAttachmentType::Color)
				internalData->colorAttachments.PushBack(attachment);
		}

		HRESULT hr;

		// Create Render Targets
		auto& attachments = descriptor->attachments->attachments;
		for (uint32 i = 0; i < attachments.Count(); i++)
		{
			if (attachments[i].type != FramebufferAttachmentType::Color)
				continue;

			D3D11_TEXTURE2D_DESC textureDesc = {};
			textureDesc.Width = texWidth;
			textureDesc.Height = texHeight;
			textureDesc.MipLevels = 1; // Usually 1 for render targets
			textureDesc.ArraySize = 1; // Single texture
			textureDesc.Format = FBFormatToDXGIFormat(attachments[i].format); // DXGI_FORMAT_R8G8B8A8_UNORM DXGI_FORMAT_R32G32B32A32_FLOAT // Use your desired DXGI_FORMAT
			textureDesc.SampleDesc.Count = descriptor->samples; // Multisampling count
			textureDesc.SampleDesc.Quality = 0; // Multisampling quality
			textureDesc.Usage = D3D11_USAGE_DEFAULT; // Default usage
			textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; // Bind as render target and shader resource
			textureDesc.CPUAccessFlags = 0; // No CPU access needed
			textureDesc.MiscFlags = 0; // No additional flags

			DXCallHR(m_Device->CreateTexture2D(&textureDesc, nullptr, &internalData->renderTextures[i]));

			DXCallHR(m_Device->CreateRenderTargetView(internalData->renderTextures[i].Get(), nullptr, &internalData->renderTargets[i]));
		}

		internalData->hasDepthStencil = descriptor->attachments->HasAttachmentType(FramebufferAttachmentType::DepthStencil);

		// Create Depth Stencil
		if (internalData->hasDepthStencil)
		{
			D3D11_DEPTH_STENCIL_DESC dsd = {};
			dsd.DepthEnable = true;
			dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			dsd.DepthFunc = D3D11_COMPARISON_LESS;

			WRL::ComPtr<ID3D11DepthStencilState> dsState;
			DXCallHR(m_Device->CreateDepthStencilState(&dsd, &dsState));

			DXCall(m_DeviceContext->OMSetDepthStencilState(dsState.Get(), 1u));

			// Depth Stencil Texture
			D3D11_TEXTURE2D_DESC td = {};
			td.Width = texWidth;
			td.Height = texHeight;
			td.MipLevels = 1;
			td.ArraySize = 1;
			td.Format = FBFormatToDXGIFormat(descriptor->attachments->GetDepthAttachmentFormat());
			td.SampleDesc.Count = 1;
			td.SampleDesc.Quality = 0;
			td.Usage = D3D11_USAGE_DEFAULT;
			td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			DXCallHR(m_Device->CreateTexture2D(&td, nullptr, &internalData->depthStencilTex));

			// Create DS View
			D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
			dsvd.Format = FBFormatToDXGIFormat(descriptor->attachments->GetDepthAttachmentFormat()); // or use DXGI_FORMAT_UNKNOWN to use the texture's
			dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsvd.Texture2D.MipSlice = 0;

			DXCallHR(m_Device->CreateDepthStencilView(internalData->depthStencilTex.Get(), &dsvd, &internalData->depthStencilView));
		}

		return fb;
	}
	void DX11Device::ResizeFramebuffer(Framebuffer* fb, uint32 width, uint32 height)
	{

	}
	void DX11Device::SetTargetFramebuffer(Framebuffer* fb, uint32 colorIndex)
	{
		if (fb == nullptr)
		{
			//DestroyRenderTarget();
			//CreateRenderTarget();
			DXCall(m_DeviceContext->OMSetRenderTargets(1, &m_RenderTarget, m_DepthStencilView.Get()));
		}
		else
		{
			auto internalData = static_cast<FramebufferDX11*>(fb->internal.get());

			ID3D11RenderTargetView* renderTargets[2] = { internalData->renderTargets[colorIndex].Get(), m_RenderTarget};
			DXCall(m_DeviceContext->OMSetRenderTargets(1, renderTargets, internalData->depthStencilView.Get()));
		}
	}
	void* DX11Device::GetFramebufferColorAttachment(Framebuffer* fb, uint32 index)
	{
		//GET_BUFFER_INTERNAL(FramebufferDX11, fb, internalData);
		// vs auto internalData = GetInternalBuffer(fb); and the function uses macros to simplify
		// function overloading to not duplicate code too much
		auto internalData = static_cast<FramebufferDX11*>(fb->internal.get());

		return internalData->renderTextures[index].Get();
	}
	void* DX11Device::GetFramebufferDepthStencilAttachment(Framebuffer* fb)
	{
		GET_BUFFER_INTERNAL(FramebufferDX11, fb, internalData);

		return internalData->depthStencilTex.Get();
	}

	RenderPass DX11Device::CreateRenderPass(RenderPassDesc* descriptor)
	{
		RenderPass pass = {};
		pass.descriptor = *descriptor;
		return pass;
	}
	void DX11Device::RCBeginRenderPass(RenderPass* pass, CommandBuffer* cmd)
	{
		const RenderPassDesc* desc = &pass->descriptor;

		SetTargetFramebuffer(desc->targetFramebuffer);
		
		if (desc->targetFramebuffer == nullptr)
		{
			RCClearColor(desc->clearColor[0], desc->clearColor[1], desc->clearColor[2], desc->clearColor[3]);
		}
		else
		{
			GET_BUFFER_INTERNAL(FramebufferDX11, pass->descriptor.targetFramebuffer, internalFB);
			for (uint32 i = 0; i < internalFB->renderTargets.Count(); i++) 
			{
				DXCall(m_DeviceContext->ClearRenderTargetView(internalFB->renderTargets[i].Get(), &desc->clearColor[0]));
			}
			if (internalFB->hasDepthStencil)
			{
				DXCall(m_DeviceContext->ClearDepthStencilView(internalFB->depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0));
			}
		}
	}
	void DX11Device::RCEndRenderPass(CommandBuffer* cmd)
	{
		SetTargetFramebuffer(nullptr); // is this even required?
	}

	//TODO: is this a good/useful function and is it worth putting it here?
	Image DX11Device::GetImageDataFromTexture(void* texture)
	{
		ID3D11Texture2D* d3dTexture = static_cast<ID3D11Texture2D*>(texture);

		// Get the description of the texture
		D3D11_TEXTURE2D_DESC textureDesc;
		d3dTexture->GetDesc(&textureDesc);

		// Create a staging texture with the same format but CPU access
		D3D11_TEXTURE2D_DESC stagingDesc = textureDesc;
		stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		stagingDesc.Usage = D3D11_USAGE_STAGING;
		stagingDesc.BindFlags = 0x00000000;

		// Create the staging texture
		WRL::ComPtr<ID3D11Texture2D> stagingTexture;

		HRESULT hr;
		DXCallHR(m_Device->CreateTexture2D(&stagingDesc, nullptr, &stagingTexture));

		// Copy the original texture to the staging texture
		DXCall(m_DeviceContext->CopyResource(stagingTexture.Get(), d3dTexture));
		// TODO: Something after here goes wrong!

		// Map the staging texture to CPU memory to read the pixels
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		//DXCall(m_DeviceContext->Flush());
		DXCallHR(m_DeviceContext->Map(stagingTexture.Get(), 0, D3D11_MAP_READ, 0, &mappedResource));

		// Create an Image object to store the pixels
		Image image;
		uint8_t* dataPtr = static_cast<uint8_t*>(mappedResource.pData);

		// Iterate through all pixels and store them in the image
		for (uint32_t y = 0; y < textureDesc.Height; ++y)
		{
			for (uint32_t x = 0; x < textureDesc.Width; ++x)
			{
				// Calculate the pixel index in the texture data (4 bytes per pixel)
				uint32_t pixelIndex = (y * mappedResource.RowPitch) + (x * 4);
				
				// Extract the BGRA components from the texture data
				uint8_t b = dataPtr[pixelIndex + 0]; //uint8_t b = 24;// = dataPtr[pixelIndex + 0];
				uint8_t g = dataPtr[pixelIndex + 1]; //uint8_t g = 134;// = dataPtr[pixelIndex + 1];
				uint8_t r = dataPtr[pixelIndex + 2]; //uint8_t r = 98;// = dataPtr[pixelIndex + 2];
				uint8_t a = dataPtr[pixelIndex + 3]; //uint8_t a= 200;// = dataPtr[pixelIndex + 3];
				
				image.pixels.PushBack(Image::Pixel(r, g, b, a));
			}
		}

		// Unmap the staging texture after reading the data
		m_DeviceContext->Unmap(stagingTexture.Get(), 0);

		return image;
	}
}
#endif