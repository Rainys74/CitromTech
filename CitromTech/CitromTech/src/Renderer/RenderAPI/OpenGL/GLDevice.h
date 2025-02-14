#pragma once

#include "Renderer/RenderAPI/GraphicsDevice.h"

//#include "DX11Includes.h"

namespace Citrom::RenderAPI
{
	class GLDevice : public Device
	{
	public:
		GLDevice();
		virtual ~GLDevice();

		GPUInfo GetCurrentGPUInfo() override;

		void WaitForGPU() override;

		// Frame Buffer (Render Target View)
		Framebuffer CreateFramebuffer(FramebufferDesc* descriptor) override {return Framebuffer();}
		void SetTargetFramebuffer(Framebuffer* fb, uint32 colorIndex = 0) override{}
		void* GetFramebufferColorAttachment(Framebuffer* fb, uint32 index = 0) override{return nullptr;}
		void* GetFramebufferDepthStencilAttachment(Framebuffer* fb) override{return nullptr;}

		Image GetImageDataFromTexture(void* texture) override{return Image();}

		void MakeSwapChain(SwapChainDesc* descriptor) override{}
		void SwapBuffers() override{}
		void SetVSync(VSyncMode vSync) override{}
		VSyncMode GetVSync() override{return VSyncMode::On;}

		void Resize(uint32 width, uint32 height) override{}
		void ResizeViewport(float32 width, float32 height, float32 xPos = 0.0f, float32 yPos = 0.0f) override{}

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

		// Pipeline
		PipelineState CreatePipelineState(PipelineStateDesc* descriptor) override { return PipelineState(); }
		void RCBindPipelineState(PipelineState* ps, CommandBuffer* cmd = nullptr) override{}

		// Render Commands
		void RCDrawIndexed(uint32 indexCount, uint32 startIndex = 0, int32 baseVertexLocation = 0) override{}
		void RCDraw(uint32 vertexCount, uint32 startVertexLocation = 0) override{}
		void RCClearColor(float32 r, float32 g, float32 b, float32 a = 0.0f) override{}

		// Debug
		void SetName(VertexBuffer* resource, const char* name) override {}
		void SetName(VertexBufferLayout* resource, const char* name) override {}
		void SetName(IndexBuffer* resource, const char* name) override {}
		void SetName(UniformBuffer* resource, const char* name) override {}
		void SetName(Texture2D* resource, const char* name) override {}
		void SetName(Shader* resource, const char* name) override {}

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
	};

	class GLDummyDevice : public DummyDevice
	{
	public:
		GLDummyDevice()
		{
			// IsOpenGLValid
			m_Valid = true; // TODO:
		}
		// TODO: Check if these are valid:
		/*
			bool IsVulkanValid()
			{
				VkInstance instance;
				VkApplicationInfo appInfo = {};
				appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
				appInfo.pApplicationName = "Validation";
				appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
				appInfo.pEngineName = "No Engine";
				appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
				appInfo.apiVersion = VK_API_VERSION_1_0;

				VkInstanceCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
				createInfo.pApplicationInfo = &appInfo;

				// Try to create Vulkan instance
				if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
				{
					// Handle failure
					return false;
				}

				// Clean up
				vkDestroyInstance(instance, nullptr);

				return true; // Vulkan instance creation successful, Vulkan is valid
			}

			bool IsOpenGLValid()
			{
				if (!glfwInit())
				{
					return false;
				}

				GLFWwindow* window = glfwCreateWindow(640, 480, "Test", nullptr, nullptr);
				if (!window)
				{
					glfwTerminate();
					return false;
				}

				glfwMakeContextCurrent(window);

				// Initialize GLEW
				if (glewInit() != GLEW_OK)
				{
					glfwDestroyWindow(window);
					glfwTerminate();
					return false;
				}

				// Check OpenGL version or extensions if needed
				const char* version = (const char*)glGetString(GL_VERSION);
				if (version)
				{
					std::cout << "OpenGL Version: " << version << std::endl;
					// You can also check if the version meets the minimum required version here
				}

				glfwDestroyWindow(window);
				glfwTerminate();

				return true; // OpenGL is valid
			}

			bool IsMetalValid()
			{
				id<MTLDevice> device = MTLCreateSystemDefaultDevice();
				if (device)
				{
					// Metal is supported and the device is valid
					return true;
				}
				else
				{
					// Metal is not supported
					return false;
				}
			}
		*/
	};
}
