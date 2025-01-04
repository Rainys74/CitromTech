#pragma once

#include "Renderer/RenderAPI/GraphicsDevice.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11Includes.h"

namespace Citrom::RenderAPI
{
	class DX11Device : public Device
	{
	public:
		DX11Device();
		virtual ~DX11Device();

		GPUInfo GetCurrentGPUInfo() override;

		// Frame Buffer (Render Target View)
		Framebuffer CreateFramebuffer(FramebufferDesc* descriptor) override { return Framebuffer(); }
		void BindFramebuffer(Framebuffer* fb) override {}

		void MakeSwapChain(SwapChainDesc* descriptor) override;
		void SwapBuffers() override;
		void SetVSync(VSyncMode vSync) override;

		void Resize(uint32 width, uint32 height) override;

		// Buffer
		VertexBuffer CreateVertexBuffer(VertexBufferDesc* descriptor) override { return VertexBuffer(); }
		void BindVertexBuffer(VertexBuffer* vb) override {}

		IndexBuffer CreateIndexBuffer(IndexBufferDesc* descriptor) override { return IndexBuffer(); }
		void BindIndexBuffer(IndexBuffer* ib) override {}

		// Render Commands
		void RCDrawIndexed(uint32 indexCount) override {}
		void RCClearColor(float32 r, float32 g, float32 b, float32 a = 0.0f) override;

		// ImGui
		void ImGuiInitGraphicsAPI() override;
		void ImGuiRenderDrawData(void* imDrawData) override;
	public:
		// DirectX11 Special
		ID3D11Device* DX11GetDevice() { return m_Device; }
		ID3D11DeviceContext* DX11GetDeviceContext() { return m_DeviceContext; }
	private:
		// Helper Functions
		void CreateRenderTarget();
		void DestroyRenderTarget();
	private:
		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_DeviceContext;
		D3D_FEATURE_LEVEL m_D3DFeatureLevel;

		WRL::ComPtr<IDXGIFactory> m_DXGIFactory;
		WRL::ComPtr<IDXGISwapChain> m_SwapChain;
		UINT m_VSyncInterval = 1;

		ID3D11RenderTargetView* m_RenderTarget;
	};
}
#endif