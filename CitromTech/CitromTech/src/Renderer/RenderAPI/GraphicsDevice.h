#pragma once

#include "Graphics.h"

#include "Framebuffer.h"
#include "Buffer.h"
#include "Shader.h"
#include "SwapChain.h"
#include "Texture.h"
#include "PipelineState.h"

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
		static constexpr const char* ToString(GraphicsAPI api)
		{
			switch (api)
			{
				case GraphicsAPI::DirectX11: return "DirectX11"; break;
				case GraphicsAPI::OpenGL: return "OpenGL"; break;
				case GraphicsAPI::Metal: return "Metal"; break;
				//case GraphicsAPI::Vulkan: return "Vulkan"; break;

				default: return "Unknown"; break;
			}
			return "Unknown";
		}
	public:
		static bool IsAPIValid(GraphicsAPI api);
	private:
		static GraphicsAPI s_GraphicsAPIList[static_cast<size_t>(GraphicsAPI::Count)];
		static GraphicsAPI& s_CurrentGraphicsAPI;
		static bool s_APIDecided;
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

		virtual void WaitForGPU() = 0;

		// Frame Buffer (Render Target View)
		virtual Framebuffer CreateFramebuffer(FramebufferDesc* descriptor) = 0;
		//virtual void SetTargetFramebuffer(Framebuffer* fb, uint32 colorIndex = 0) = 0;
		virtual void ResizeFramebuffer(Framebuffer* fb, uint32 width = 0, uint32 height = 0) = 0; // 0 will use the screen size
		virtual void* GetFramebufferColorAttachment(Framebuffer* fb, uint32 index = 0) = 0;
		virtual void* GetFramebufferDepthStencilAttachment(Framebuffer* fb) = 0;

        virtual RenderPass CreateRenderPass(RenderPassDesc* descriptor) = 0;
        virtual void RCBeginRenderPass(RenderPass* pass, CommandBuffer* cmd = nullptr) = 0;
        virtual void RCEndRenderPass(CommandBuffer* cmd = nullptr) = 0;

		virtual Image GetImageDataFromTexture(void* texture) = 0;

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
		virtual VSyncMode GetVSync() = 0;

		virtual void Resize(uint32 width, uint32 height) = 0;
		virtual void RCSetViewport(const ViewportSpecification& viewport, CommandBuffer* cmd = nullptr) = 0;

		// Vertex Buffer
		virtual VertexBuffer CreateVertexBuffer(VertexBufferDesc* descriptor) = 0;
		virtual void RCBindVertexBuffer(VertexBuffer* vb, CommandBuffer* cmd = nullptr) = 0; // TODO: how to separate render commands and command list/encoder/queue/buffer functions? Probably make Render Commands hold a default command list pointer as nullptr which uses immediate mode CB's/CL's/CE's/CQ's

		virtual VertexBufferLayout CreateVertexBufferLayout(VertexBufferLayoutDesc* descriptor) = 0;
		//virtual void BindVertexBufferLayout(VertexBufferLayout* vbLayout) = 0; // TODO: comment out and move to private helper function in dx11

		virtual IndexBuffer CreateIndexBuffer(IndexBufferDesc* descriptor) = 0;
		virtual void RCBindIndexBuffer(IndexBuffer* ib, CommandBuffer* cmd = nullptr) = 0;

		// Shader
		virtual Shader CreateShader(ShaderDesc* descriptor) = 0;
		virtual ComputeShader CreateComputeShader(ShaderDesc* descriptor) { return ComputeShader(); }
		virtual void RCDispatchCompute(uint32 workGroupsX, uint32 workGroupsY, uint32 workGroupsZ = 1, CommandBuffer* cmd = nullptr) {}
		//virtual void BindShader(Shader* shader) = 0; // TODO: comment out and move to private helper function in dx11

		virtual UniformBuffer CreateUniformBuffer(UniformBufferDesc* descriptor) = 0;
		virtual void RCBindUniformBuffer(UniformBuffer* ub, ShaderType shaderStage = ShaderType::Vertex, uint32 startSlot = 0, CommandBuffer* cmd = nullptr) = 0; // TODO: i'm pretty sure other api's don't require separate shader stages...
		virtual void SetUniformBufferData(UniformBuffer* ub, const void* data, const size_t size) = 0;

		// Textures
		virtual Texture2D CreateTexture2D(Texture2DDesc* descriptor) = 0;
		virtual void RCBindTexture2D(Texture2D* tex2D, uint32 startSlot = 0, CommandBuffer* cmd = nullptr) = 0;

		// Pipeline
		virtual PipelineState CreatePipelineState(PipelineStateDesc* descriptor) = 0;
		virtual void RCBindPipelineState(PipelineState* ps, CommandBuffer* cmd = nullptr) = 0;

		// Command Buffers (also known as Command Lists, Command Encoders and sometimes Command Queues)
		virtual CommandBuffer CreateCommandBuffer() { return CommandBuffer(); }
		virtual void BeginCommandBuffer(CommandBuffer* cmd) {} // or should this be combined with Create? Probably not.
		virtual void SubmitCommandBuffer(CommandBuffer* cmd) {}
		virtual void ResetCommandBuffer(CommandBuffer* cmd) {}

		// Render Commands
		virtual void RCBegin() {}
		virtual void RCEnd() {}
		virtual void RCDrawIndexed(uint32 indexCount, uint32 startIndex = 0, int32 baseVertexLocation = 0, CommandBuffer* cmd = nullptr) = 0; // RCBegin and RCEnd to provide a high-level interface of command buffers/lists in Metal/Vulkan? also deferred DX11 Contexts?
		virtual void RCDraw(uint32 vertexCount, uint32 startVertexLocation = 0, CommandBuffer* cmd = nullptr) = 0;
		virtual void RCClearColor(float32 r, float32 g, float32 b, float32 a = 0.0f) = 0; // TODO: move to render pass as a value, or keep 

		// Debug (Name setting)
		virtual void SetName(VertexBuffer* resource, const char* name) = 0;
		virtual void SetName(VertexBufferLayout* resource, const char* name) = 0;
		virtual void SetName(IndexBuffer* resource, const char* name) = 0;
		virtual void SetName(UniformBuffer* resource, const char* name) = 0;
		virtual void SetName(Texture2D* resource, const char* name) = 0;
		virtual void SetName(Shader* resource, const char* name) = 0;

		virtual void RCPushDebugGroup(const char* name, CommandBuffer* cmd = nullptr) = 0; // Begin Event/Push Debug Event/Push Debug Group
		virtual void RCPopDebugGroup(CommandBuffer* cmd = nullptr) = 0;

		// ImGui
		virtual void ImGuiInitGraphicsAPI() = 0;
        virtual void ImGuiNewFrame(CommandBuffer* cmd = nullptr) = 0;
		virtual void ImGuiRenderDrawData(void* imDrawData, CommandBuffer* cmd = nullptr) = 0;
	private:
		// Device Factory
		static Device* CreateDevice();
		static Device* s_Instance;
	protected:
		// Helper functions
		size_t GetFormatSize(Format format);
		size_t GetFBFormatSize(FramebufferFormat format);
		size_t GetLayoutStride(const VertexBufferLayoutDesc* vbLayoutSpec);

		Format FBFormatToFormat(FramebufferFormat fbFormat);
        
        uint32 CalculateMipLevels(uint32 width, uint32 height)
        {
            return (uint32)log2(std::max(width, height)) + 1;
        }
	protected:
		uint32 m_Width, m_Height;
	};

	#define DEVICE (Citrom::RenderAPI::Device::Get())

	#ifdef CT_DEBUG
	#define Device_SetDebugName(RESOURCE, NAME) DEVICE->SetName(RESOURCE, NAME)

	#define Device_PushDebugGroup(NAME, ...) DEVICE->RCPushDebugGroup(NAME, ##__VA_ARGS__)
	#define Device_PopDebugGroup(...) DEVICE->RCPopDebugGroup(__VA_ARGS__);
	#else
	#define Device_SetDebugName(...)

	#define Device_PushDebugGroup(...)
	#define Device_PopDebugGroup(...)
	#endif

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
