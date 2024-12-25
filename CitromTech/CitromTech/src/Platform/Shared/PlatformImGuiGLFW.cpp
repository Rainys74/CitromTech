#include "Platform/PlatformImGui.h"

#ifndef CT_PLATFORM_WINDOWS

#include "CitromAssert.h"
#include "Renderer/Renderer.h"

#include "imgui.h"

#include "backends/imgui_impl_glfw.h"

#include "backends/imgui_impl_opengl3.h"
//#include "backends/imgui_impl_metal.h"

namespace Citrom::Platform
{
	Window* ImGui::m_Window;

	void ImGui::Initialize(Window* window)
	{
		if (RenderAPI::GraphicsAPIManager::IsGraphicsAPI(RenderAPI::GraphicsAPI::OpenGL))
		{
			CT_CORE_VERIFY(ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->GLFWTryGetWnd(), false), "Failed to Initialize ImGui GLFW implementation for OpenGL.");
			CT_CORE_VERIFY(ImGui_ImplOpenGL3_Init("#version 410"), "Failed to Initialize ImGui OpenGL 4.1 implementation.");
		}

		Renderer::ImGuiInit();
	}
	void ImGui::Terminate()
	{
		if (RenderAPI::GraphicsAPIManager::IsGraphicsAPI(RenderAPI::GraphicsAPI::OpenGL))
		{
			ImGui_ImplOpenGL3_Shutdown();
		}

		ImGui_ImplGlfw_Shutdown();
	}

	void ImGui::NewFrame()
	{
		if (RenderAPI::GraphicsAPIManager::IsGraphicsAPI(RenderAPI::GraphicsAPI::OpenGL))
		{
			ImGui_ImplOpenGL3_NewFrame();
		}
	}
}
#endif