#pragma once

#include "Graphics.h"

#include "Framebuffer.h"
#include "Buffer.h"

namespace Citrom::RenderAPI
{
	class GraphicsAPIManager
	{
	public:
		static GraphicsAPI GetGraphicsAPI();
		static bool IsGraphicsAPI(GraphicsAPI graphicsAPI);

		static void ForceGraphicsAPI(GraphicsAPI graphicsAPI);
		static void PrioritizeGraphicsAPI(GraphicsAPI graphicsAPI, uint8 priorityLevel) {}
	private:
		static GraphicsAPI s_CurrentGraphicsAPI;
	};

	class Device
	{
	public:
		Device() {}
		virtual ~Device() {}

		// Device Factory
		static Device* CreateDevice();
		
		// Frame Buffer (Render Target View)
		virtual Framebuffer CreateFramebuffer(FramebufferDesc* descriptor) = 0;
		virtual void BindFramebuffer(Framebuffer* fb) = 0;
		// GetColorAttachmentRendererID? to return texture id

		// Swap Chain
		// Thoughts?: user technically should have access to/own the swap chain, but
		// the device would be a better owner because every time you for example
		// resize the window the user needs to resize the swapchain, and that's pretty annoying
		// maybe Device should be responsible for that and to listen to the event?
		// e.g. Torque3D holds an internal swap-chain and RTV's inside D3D11's Gfx
		// Wicked Engine holds an internal swap-chain and RTV's inside a SwapChain class and it's internal ref
		//virtual void MakeSwapChain(SwapChainDesc* descriptor) = 0;
		//virtual void SwapBuffers(SwapChain* sc) = 0; // implemented in device for better error checking on DX11

		// Vertex Buffer
		// TODO: figure out whether to return or set using pointers
		virtual VertexBuffer CreateVertexBuffer(VertexBufferDesc* descriptor) = 0;
		virtual void BindVertexBuffer(VertexBuffer* vb) = 0;

		virtual IndexBuffer CreateIndexBuffer(IndexBufferDesc* descriptor) = 0;
		virtual void BindIndexBuffer(IndexBuffer* ib) = 0;

		// Render Commands
		virtual void RCDrawIndexed(uint32 indexCount) = 0;
		virtual void RCClearColor(float32 r, float32 g, float32 b, float32 a = 0.0f) = 0;

		// ImGui
		virtual void ImGuiInitGraphicsAPI() = 0;
		virtual void ImGuiRenderDrawData() = 0;
	protected:
	};
}