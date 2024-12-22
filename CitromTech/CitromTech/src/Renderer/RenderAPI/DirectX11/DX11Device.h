#pragma once

#include "Renderer/RenderAPI/GraphicsDevice.h"

#ifdef CT_PLATFORM_WINDOWS
#include "DX11Includes.h"

namespace Citrom::RenderAPI
{
	class DX11Device : public Device
	{
	public:
		DX11Device() {}
		virtual ~DX11Device() {}

		// Frame Buffer (Render Target View)
		Framebuffer CreateFramebuffer(FramebufferDesc* descriptor) override { return Framebuffer(); }
		void BindFramebuffer(Framebuffer* fb) override {}

		// Buffer
		VertexBuffer CreateVertexBuffer(VertexBufferDesc* descriptor) override { return VertexBuffer(); }
		void BindVertexBuffer(VertexBuffer* vb) override {}

		IndexBuffer CreateIndexBuffer(IndexBufferDesc* descriptor) override { return IndexBuffer(); }
		void BindIndexBuffer(IndexBuffer* ib) override {}

		// Render Commands
		void RCDrawIndexed(uint32 indexCount) override {}
		void RCClearColor(float32 r, float32 g, float32 b, float32 a = 0.0f) override {}

		// ImGui
		void ImGuiInitGraphicsAPI() override;
		void ImGuiRenderDrawData() override;
	private:
	};
}
#endif