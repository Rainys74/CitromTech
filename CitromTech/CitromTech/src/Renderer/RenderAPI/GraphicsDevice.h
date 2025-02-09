#pragma once

#include "Graphics.h"

#include "Framebuffer.h"
#include "Buffer.h"
#include "Shader.h"
#include "SwapChain.h"
#include "Texture.h"

// Also referred to as the Render Hardware Interface (RHI) in engines such as Unreal Engine and O3DE
namespace Citrom::RenderAPI
{
	class GraphicsAPIManager
	{
	public:
		static GraphicsAPI GetGraphicsAPI();
		static bool IsGraphicsAPI(GraphicsAPI graphicsAPI);

		static void ForceGraphicsAPI(GraphicsAPI graphicsAPI);
		static void PrioritizeGraphicsAPI(GraphicsAPI graphicsAPI, uint8 priorityLevel = 0);

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
				s_Instance = CreateDevice(); // TODO: probably move this into a different function to make sure the device doesn't get initialized before api selection
			}
			return s_Instance;
		}
		
		// Because of OpenGL's lack of an Adapter system: i decided
		// it's not my job to pick the GPU for the user, basically Gets the Current Adapter's information
		virtual GPUInfo GetCurrentGPUInfo() = 0;

		// Frame Buffer (Render Target View)
		virtual Framebuffer CreateFramebuffer(FramebufferDesc* descriptor) = 0;
		virtual void SetTargetFramebuffer(Framebuffer* fb, uint32 colorIndex = 0) = 0;
		virtual void* GetFramebufferColorAttachment(Framebuffer* fb, uint32 index = 0) = 0;
		virtual void* GetFramebufferDepthStencilAttachment(Framebuffer* fb) = 0;

		virtual Image GetImageDataFromTexture(void* texture) = 0;

		// Swap Chain
		// Thoughts?: user technically should have access to/own the swap chain, but
		// the device would be a better owner because every time you for example
		// resize the window the user needs to resize the swapchain, and that's pretty annoying
		// maybe Device should be responsible for that and to listen to the event?
		// e.g. Torque3D holds an internal swap-chain and RTV's inside D3D11's Gfx (This)
		// Wicked Engine holds an internal swap-chain and RTV's inside a SwapChain class and it's internal ref
		virtual void MakeSwapChain(SwapChainDesc* descriptor, BlendStateDesc* blendSpec = nullptr, RasterizerStateDesc* rasterDesc = nullptr) = 0; // TODO: Create pipeline for states to simulate Vulkan/Metal
		virtual void SwapBuffers() = 0; // implemented in device for better error checking on DX11
		virtual void SetVSync(VSyncMode vSync) = 0; // implemented in device for better error checking on DX11
		virtual VSyncMode GetVSync() = 0;

		virtual void Resize(uint32 width, uint32 height) = 0;
		virtual void ResizeViewport(uint32 width, uint32 height, int32 xPos = 0, int32 yPos = 0) = 0;

		// Vertex Buffer
		// TODO: figure out whether to return or set using pointers
		virtual VertexBuffer CreateVertexBuffer(VertexBufferDesc* descriptor) = 0;
		virtual void BindVertexBuffer(VertexBuffer* vb) = 0;

		virtual VertexBufferLayout CreateVertexBufferLayout(VertexBufferLayoutDesc* descriptor) = 0;
		virtual void BindVertexBufferLayout(VertexBufferLayout* vbLayout) = 0;

		virtual IndexBuffer CreateIndexBuffer(IndexBufferDesc* descriptor) = 0;
		virtual void BindIndexBuffer(IndexBuffer* ib) = 0;

		// Shader
		virtual Shader CreateShader(ShaderDesc* descriptor) = 0;
		virtual void BindShader(Shader* shader) = 0;

		virtual UniformBuffer CreateUniformBuffer(UniformBufferDesc* descriptor) = 0;
		virtual void BindUniformBuffer(UniformBuffer* ub, ShaderType shaderStage = ShaderType::Vertex, uint32 startSlot = 0) = 0;
		virtual void SetUniformBufferData(UniformBuffer* ub, const void* data, const size_t size) = 0;

		// Textures
		virtual Texture2D CreateTexture2D(Texture2DDesc* descriptor) = 0;
		virtual void BindTexture2D(Texture2D* tex2D, uint32 startSlot = 0) = 0;

		// Render Commands
		virtual void RCDrawIndexed(uint32 indexCount, uint32 startIndex = 0, int32 baseVertexLocation = 0) = 0;
		virtual void RCDraw(uint32 vertexCount, uint32 startVertexLocation = 0) = 0;
		virtual void RCClearColor(float32 r, float32 g, float32 b, float32 a = 0.0f) = 0;

		// ImGui
		virtual void ImGuiInitGraphicsAPI() = 0;
        virtual void ImGuiNewFrame() = 0;
		virtual void ImGuiRenderDrawData(void* imDrawData) = 0;
	private:
		// Device Factory
		static Device* CreateDevice();
		static Device* s_Instance;
	protected:
		// Helper functions
		size_t GetFormatSize(Format format);
		size_t GetFBFormatSize(FramebufferFormat format);
		size_t GetLayoutStride(const VertexBufferLayoutDesc* vbLayoutSpec);
	protected:
		uint32 m_Width, m_Height;
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

	// Macros for easier device backend development
	#define GET_BUFFER_INTERNAL(INTERNTYPE, BUFFERPTRNAME, INTERNDATANAME) auto INTERNDATANAME = static_cast<INTERNTYPE*>((BUFFERPTRNAME)->internal.get())

	//GET_BUFFER_INTERNAL(FramebufferDX11, fb, internalData);
	//auto internalData = static_cast<FramebufferDX11*>((fb)->internal.get());
	//auto internalData = static_cast<FramebufferDX11*>(fb->internal.get());

	#define CREATE_BUFFER_INTERNAL_NO_DESC(BUFFERTYPE, BUFFERINTERNTYPE, BUFFEROBJNAME, INTERNDATANAME)		BUFFERTYPE BUFFEROBJNAME; \
																											BUFFEROBJNAME.internal = CTL::CreateRef<BUFFERINTERNTYPE>(); \
																											auto INTERNDATANAME = static_cast<BUFFERINTERNTYPE*>(BUFFEROBJNAME.internal.get())

	#define CREATE_BUFFER_INTERNAL(BUFFERTYPE, BUFFERINTERNTYPE, BUFFEROBJNAME, INTERNDATANAME)	CREATE_BUFFER_INTERNAL_NO_DESC(BUFFERTYPE, BUFFERINTERNTYPE, BUFFEROBJNAME, INTERNDATANAME);	\
																								BUFFEROBJNAME.descriptor = *descriptor  // CANNOT COPY DESCRIPTOR IF IT INCLUDES ALLOCATED MEMORY FOR EXAMPLE DARRAY IN MOST CASES!

	//CREATE_BUFFER_INTERNAL(Framebuffer, FramebufferDX11, fb, internalData);
	//
	//Framebuffer fb; 
	//fb.internal = CTL::CreateRef<FramebufferDX11>(); 
	//auto internalData = static_cast<FramebufferDX11*>(fb.internal.get()); 
	//fb.descriptor = *descriptor;;
	//
	//Framebuffer fb;
	//fb.internal = CTL::CreateRef<FramebufferDX11>();
	//auto internalData = static_cast<FramebufferDX11*>(fb.internal.get());
	//fb.descriptor = *descriptor;
}
