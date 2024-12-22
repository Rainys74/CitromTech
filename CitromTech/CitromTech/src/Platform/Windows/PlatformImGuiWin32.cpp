#include "Platform/PlatformImGui.h"

#ifdef CT_PLATFORM_WINDOWS

#include "CitromAssert.h"
#include "Renderer/RenderAPI/GraphicsDevice.h"

#include "imgui.h"

#include "backends/imgui_impl_win32.h"

#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_opengl3.h"

namespace Citrom::Platform
{
	Window* ImGui::m_Window;

	void ImGui::Initialize(Window* window)
	{
		m_Window = window;

		if (RenderAPI::GraphicsAPIManager::IsGraphicsAPI(RenderAPI::GraphicsAPI::DirectX11))
		{
			ImGui_ImplWin32_Init(window->Win32TryGetHWnd());
			//ImGui_ImplDX11_Init(); // TODO: move this over to the renderer probably
		}
		else if (RenderAPI::GraphicsAPIManager::IsGraphicsAPI(RenderAPI::GraphicsAPI::OpenGL))
		{
			// if win32 & opengl
			CT_CORE_VERIFY(ImGui_ImplWin32_InitForOpenGL(window->Win32TryGetHWnd()), "Failed to Initialize ImGui Win32 implementation for OpenGL.");
			CT_CORE_VERIFY(ImGui_ImplOpenGL3_Init("#version 460"), "Failed to Initialize ImGui OpenGL 4.6 implementation.");
		}
	}
	void ImGui::Terminate()
	{
		if (RenderAPI::GraphicsAPIManager::IsGraphicsAPI(RenderAPI::GraphicsAPI::DirectX11))
		{
			ImGui_ImplDX11_Shutdown();
		}
		else if (RenderAPI::GraphicsAPIManager::IsGraphicsAPI(RenderAPI::GraphicsAPI::OpenGL))
		{
			ImGui_ImplOpenGL3_Shutdown();
		}

		ImGui_ImplWin32_Shutdown();
	}

	void ImGui::NewFrame()
	{
		if (RenderAPI::GraphicsAPIManager::IsGraphicsAPI(RenderAPI::GraphicsAPI::DirectX11))
		{
			ImGui_ImplDX11_NewFrame();
		}
		else if (RenderAPI::GraphicsAPIManager::IsGraphicsAPI(RenderAPI::GraphicsAPI::OpenGL))
		{
			ImGui_ImplOpenGL3_NewFrame();
			//ImGui_ImplOpenGL3_RenderDrawData(::ImGui::GetDrawData());
		}
	}
}
#endif