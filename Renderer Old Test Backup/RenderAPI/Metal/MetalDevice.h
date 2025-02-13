#pragma once

#include "Renderer/RenderAPI/GraphicsDevice.h"

#ifdef CT_PLATFORM_MACOS
#import <Metal/Metal.h>

namespace Citrom::RenderAPI
{
	class MetalDevice : public Device
	{
	public:
        MetalDevice();
		virtual ~MetalDevice();

		GPUInfo GetCurrentGPUInfo() override;

		// Frame Buffer (Render Target View)
		Framebuffer CreateFramebuffer(FramebufferDesc* descriptor) override {return Framebuffer();}
		void SetTargetFramebuffer(Framebuffer* fb, uint32 colorIndex = 0) override{}
		void* GetFramebufferColorAttachment(Framebuffer* fb, uint32 index = 0) override{return nullptr;}
		void* GetFramebufferDepthStencilAttachment(Framebuffer* fb) override{return nullptr;}

		Image GetImageDataFromTexture(void* texture) override{return Image();}

        void MakeSwapChain(SwapChainDesc* descriptor, BlendStateDesc* blendSpec = nullptr, RasterizerStateDesc* rasterDesc = nullptr) override
        {
            m_Width = descriptor->windowPtr->GetBackend()->GetWidth();
            m_Height = descriptor->windowPtr->GetBackend()->GetHeight();
        }
		void SwapBuffers() override{}
		void SetVSync(VSyncMode vSync) override{}
		VSyncMode GetVSync() override{return VSyncMode::On;}

        void Resize(uint32 width, uint32 height) override{m_Width = width; m_Height = height;}
		void ResizeViewport(uint32 width, uint32 height, int32 xPos = 0, int32 yPos = 0) override{}

		// Buffer
		VertexBuffer CreateVertexBuffer(VertexBufferDesc* descriptor) override{return VertexBuffer();}
		void BindVertexBuffer(VertexBuffer* vb) override{}

		VertexBufferLayout CreateVertexBufferLayout(VertexBufferLayoutDesc* descriptor) override{return VertexBufferLayout();}
		void BindVertexBufferLayout(VertexBufferLayout* vbLayout) override{}

		IndexBuffer CreateIndexBuffer(IndexBufferDesc* descriptor) override{return IndexBuffer();}
		void BindIndexBuffer(IndexBuffer* ib) override{}

		// Shader
		Shader CreateShader(ShaderDesc* descriptor) override{return Shader();}
		void BindShader(Shader* shader) override{}

		UniformBuffer CreateUniformBuffer(UniformBufferDesc* descriptor) override{return UniformBuffer();}
		void BindUniformBuffer(UniformBuffer* ub, ShaderType shaderStage = ShaderType::Vertex, uint32 startSlot = 0) override{}
		void SetUniformBufferData(UniformBuffer* ub, const void* data, const size_t size) override{}

		// Textures
		Texture2D CreateTexture2D(Texture2DDesc* descriptor) override{return Texture2D();}
		void BindTexture2D(Texture2D* tex2D, uint32 startSlot = 0) override{}

		// Render Commands
		void RCDrawIndexed(uint32 indexCount, uint32 startIndex = 0, int32 baseVertexLocation = 0) override{}
		void RCDraw(uint32 vertexCount, uint32 startVertexLocation = 0) override{}
		void RCClearColor(float32 r, float32 g, float32 b, float32 a = 0.0f) override{}

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
	private:
        id<MTLDevice> m_Device;
        
        id<MTLCommandBuffer> m_CommandBuffer;
        id<MTLRenderCommandEncoder> m_CommandEncoder;
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
}
#endif
