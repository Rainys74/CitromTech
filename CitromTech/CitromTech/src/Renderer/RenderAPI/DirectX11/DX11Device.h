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

		IndexBuffer CreateIndexBuffer(IndexBufferDesc* descriptor) override;
		void BindIndexBuffer(IndexBuffer* ib) override;

		// Render Commands
		void RCDrawIndexed(uint32 indexCount) override;
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

	class DX11DummyDevice : public DummyDevice
	{
	public:
		DX11DummyDevice()
		{
			// IsDX11Valid
			ID3D11Device* device = nullptr;
			ID3D11DeviceContext* context = nullptr;
			D3D_FEATURE_LEVEL featureLevel;

			HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0x00000000, nullptr, 0, D3D11_SDK_VERSION, &device, &featureLevel, &context);

			m_Valid = !FAILED(hr);

			SAFE_RELEASE(device);
			SAFE_RELEASE(context);
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
#endif