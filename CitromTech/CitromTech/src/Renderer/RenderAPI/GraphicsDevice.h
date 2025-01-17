#pragma once

#include "Graphics.h"

#include "Framebuffer.h"
#include "Buffer.h"
#include "SwapChain.h"

// Also referred to as the Render Hardware Interface (RHI) in engines such as Unreal Engine and O3DE
namespace Citrom::RenderAPI
{
	class GraphicsAPIManager
	{
	public:
		static GraphicsAPI GetGraphicsAPI();
		static bool IsGraphicsAPI(GraphicsAPI graphicsAPI);

		static void ForceGraphicsAPI(GraphicsAPI graphicsAPI);
		static void PrioritizeGraphicsAPI(GraphicsAPI graphicsAPI, uint8 priorityLevel);

		static GraphicsAPI GetGraphicsAPIAtPriority(uint8 priorityLevel);
	public:
		static bool IsAPIValid(GraphicsAPI api);
	private:
		static GraphicsAPI s_GraphicsAPIList[static_cast<size_t>(GraphicsAPI::Count)];
		static GraphicsAPI& s_CurrentGraphicsAPI;
	};

	// Device is a singleton class to better support OpenGL's single device implementation
	class Device
	{
	public:
		Device() {}
		virtual ~Device() {}

		// Device Singleton
		static Device* Get()
		{
			if (!s_Instance)
			{
				s_Instance = CreateDevice();
			}
			return s_Instance;
		}
		
		// Because of OpenGL's lack of an Adapter system: i decided
		// it's not my job to pick the GPU for the user, basically Gets the Current Adapter's information
		virtual GPUInfo GetCurrentGPUInfo() = 0;

		// Frame Buffer (Render Target View)
		virtual Framebuffer CreateFramebuffer(FramebufferDesc* descriptor) = 0;
		virtual void BindFramebuffer(Framebuffer* fb) = 0;
		// GetColorAttachmentRendererID? to return texture id

		// Swap Chain
		// Thoughts?: user technically should have access to/own the swap chain, but
		// the device would be a better owner because every time you for example
		// resize the window the user needs to resize the swapchain, and that's pretty annoying
		// maybe Device should be responsible for that and to listen to the event?
		// e.g. Torque3D holds an internal swap-chain and RTV's inside D3D11's Gfx (This)
		// Wicked Engine holds an internal swap-chain and RTV's inside a SwapChain class and it's internal ref
		virtual void MakeSwapChain(SwapChainDesc* descriptor) = 0;
		virtual void SwapBuffers() = 0; // implemented in device for better error checking on DX11
		virtual void SetVSync(VSyncMode vSync) = 0; // implemented in device for better error checking on DX11

		virtual void Resize(uint32 width, uint32 height) = 0;

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
		virtual void ImGuiRenderDrawData(void* imDrawData) = 0;
	private:
		// Device Factory
		static Device* CreateDevice();
		static Device* s_Instance;
	protected:
	};

	// Device for checking the validity of the API, do not use directly! only used for inheritance.
	class DummyDevice
	{
	public:
		virtual ~DummyDevice() = default;

		virtual bool IsValid() const { return m_Valid; }
	protected:
		bool m_Valid = false;
	};
}