#pragma once

#include "Renderer/RenderAPI/GraphicsDevice.h"

#ifdef CT_PLATFORM_MACOS
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#import <MetalKit/MetalKit.h>

namespace Citrom::RenderAPI
{
	class MetalDevice : public Device
	{
	public:
        MetalDevice();
		virtual ~MetalDevice();

		GPUInfo GetCurrentGPUInfo() override;
        
        void WaitForGPU() override {}

		// Frame Buffer (Render Target View)
        Framebuffer CreateFramebuffer(FramebufferDesc* descriptor) override;
		void* GetFramebufferColorAttachment(Framebuffer* fb, uint32 index = 0) override{return nullptr;}
		void* GetFramebufferDepthStencilAttachment(Framebuffer* fb) override{return nullptr;}

        RenderPass CreateRenderPass(RenderPassDesc* descriptor) override;
        void RCBeginRenderPass(RenderPass* pass, CommandBuffer* cmd = nullptr) override;
        void RCEndRenderPass(CommandBuffer* cmd = nullptr) override;
        
		Image GetImageDataFromTexture(void* texture) override{return Image();}

        void MakeSwapChain(SwapChainDesc* descriptor) override
        {
            m_Width = descriptor->windowPtr->GetBackend()->GetWidth();
            m_Height = descriptor->windowPtr->GetBackend()->GetHeight();
            
            NSView* cocoaView = (NSView*)descriptor->windowPtr->GetBackend()->CocoaTryGetNSView();
            
            if (![cocoaView.layer isKindOfClass:[CAMetalLayer class]])
                cocoaView.layer = [CAMetalLayer layer];
            
            if ([cocoaView.layer isKindOfClass:[CAMetalLayer class]])
                m_MTLLayer = (CAMetalLayer*)cocoaView.layer;
                
            m_MTLLayer.device = m_Device;
            m_MTLLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
            m_MTLLayer.drawableSize = CGSizeMake(m_Width, m_Height);
            
            //m_Drawable = [m_MTLLayer nextDrawable];
            
            //m_MTKView = [[MTKView alloc] initWithFrame:cocoaView.bounds device:m_Device]; // do i need to clear this?
            //[cocoaView addSubview:m_MTKView];
        }
		void SwapBuffers() override{}
		void SetVSync(VSyncMode vSync) override
        {
            switch (vSync)
            {
                default:
                case VSyncMode::On:
                    m_MTLLayer.displaySyncEnabled = YES;
                    break;
                case VSyncMode::Off:
                    m_MTLLayer.displaySyncEnabled = NO;
                    break;
            }
        }
		VSyncMode GetVSync() override
        {
            return m_MTLLayer.displaySyncEnabled ? VSyncMode::On : VSyncMode::Off;
        }

        void Resize(uint32 width, uint32 height) override
        {
            m_Width = width;
            m_Height = height;
            
            m_MTLLayer.drawableSize = CGSizeMake(m_Width, m_Height);
        }
        void ResizeViewport(float32 width, float32 height, float32 xPos = 0.0f, float32 yPos = 0.0f) override{}

        // Buffer
        VertexBuffer CreateVertexBuffer(VertexBufferDesc* descriptor) override;
        void RCBindVertexBuffer(VertexBuffer* vb, CommandBuffer* cmd = nullptr) override;

        VertexBufferLayout CreateVertexBufferLayout(VertexBufferLayoutDesc* descriptor) override{return VertexBufferLayout();}

        IndexBuffer CreateIndexBuffer(IndexBufferDesc* descriptor) override;
        void RCBindIndexBuffer(IndexBuffer* ib, CommandBuffer* cmd = nullptr) override;

        // Shader
        Shader CreateShader(ShaderDesc* descriptor) override;

        UniformBuffer CreateUniformBuffer(UniformBufferDesc* descriptor) override;
        void RCBindUniformBuffer(UniformBuffer* ub, ShaderType shaderStage = ShaderType::Vertex, uint32 startSlot = 0, CommandBuffer* cmd = nullptr) override;
        void SetUniformBufferData(UniformBuffer* ub, const void* data, const size_t size) override;

        // Textures
        Texture2D CreateTexture2D(Texture2DDesc* descriptor) override{return Texture2D();}
        void RCBindTexture2D(Texture2D* tex2D, uint32 startSlot = 0, CommandBuffer* cmd = nullptr) override{}

        // Pipeline
        PipelineState CreatePipelineState(PipelineStateDesc* descriptor) override;
        void RCBindPipelineState(PipelineState* ps, CommandBuffer* cmd = nullptr) override;
        
        // Command Buffers
        CommandBuffer CreateCommandBuffer() override;
        void BeginCommandBuffer(CommandBuffer* cmd) override;
        void SubmitCommandBuffer(CommandBuffer* cmd) override;
        void ResetCommandBuffer(CommandBuffer* cmd) override {}

        // Render Commands
        void RCBegin() override;
        void RCEnd() override;
        void RCDrawIndexed(uint32 indexCount, uint32 startIndex = 0, int32 baseVertexLocation = 0, CommandBuffer* cmd = nullptr) override;
        void RCDraw(uint32 vertexCount, uint32 startVertexLocation = 0, CommandBuffer* cmd = nullptr) override;
        void RCClearColor(float32 r, float32 g, float32 b, float32 a = 0.0f) override{}

        // Debug
        void SetName(VertexBuffer* resource, const char* name) override {}
        void SetName(VertexBufferLayout* resource, const char* name) override {}
        void SetName(IndexBuffer* resource, const char* name) override {}
        void SetName(UniformBuffer* resource, const char* name) override {}
        void SetName(Texture2D* resource, const char* name) override {}
        void SetName(Shader* resource, const char* name) override {}

        void RCPushDebugGroup(const char* name, CommandBuffer* cmd = nullptr) override {}
        void RCPopDebugGroup(CommandBuffer* cmd = nullptr) override {}

		// ImGui
        void ImGuiInitGraphicsAPI() override;
		void ImGuiNewFrame() override;
		void ImGuiRenderDrawData(void* imDrawData) override;
	public:
/*
		DXGI_FORMAT FormatToDXGIFormat(Format format);
		//Format DXGIFormatToFormat(DXGI_FORMAT dxgiFormat);
		D3D11_USAGE UsageToD3D11Usage(Usage usage);

		// Framebuffer
		DXGI_FORMAT FBFormatToDXGIFormat(FramebufferFormat format);

		// Blending
		D3D11_BLEND BlendFactorToD3D11Blend(BlendFactor factor);
		D3D11_BLEND_OP BlendOpToD3D11BlendOp(BlendOp blendOp);
		UINT RenderTargetWriteMaskToD3D11(RenderTargetWriteMask mask);
		
		// Rasterizer
		D3D11_FILL_MODE FillModeToD3D11FillMode(FillMode fillMode);
		D3D11_CULL_MODE CullModeToD3D11CullMode(CullMode cullMode);
	private:
		// Helper Functions
		void CreateRenderTarget();
		void DestroyRenderTarget();

		// Function Overloading To convert buffers to internal buffers
		// hmm no can't do because internal structs are declared in .cpp files.. unless static?*/
    protected:
        static CommandBuffer s_RenderCommandBuffer;
	private:
        id<MTLDevice> m_Device;
        id<MTLCommandQueue> m_CommandQueue;
        
        CAMetalLayer* m_MTLLayer;
        id<CAMetalDrawable> m_Drawable;
        
        id<MTLBuffer>* m_CurrentIndexBuffer = nullptr; // TODO: would a copy be more useful and more similar to other apis?
        
        id<MTLCommandBuffer> m_ImCommandBuffer;
        id<MTLRenderCommandEncoder> m_ImCommandEncoder;
	};

	class MetalDummyDevice : public DummyDevice
	{
	public:
        MetalDummyDevice()
		{
			// IsMetalValid
            id<MTLDevice> device = MTLCreateSystemDefaultDevice();
            m_Valid = (device != nil);
		}
	};

    struct CommandBufferMTL
    {
        id<MTLCommandBuffer> commandBuffer;
        id<MTLRenderCommandEncoder> commandEncoder;
        
        ~CommandBufferMTL()
        {
            [commandEncoder release];
            [commandBuffer release];
        }
    };

    struct ShaderMTL
    {
        id<MTLFunction> vertexFunction;
        id<MTLFunction> fragmentFunction;
        
        ~ShaderMTL()
        {
            [vertexFunction release];
            [fragmentFunction release];
        }
    };
}
#endif
