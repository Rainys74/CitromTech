#include "Platform/PlatformImGui.h"

#ifdef CT_PLATFORM_WINDOWS

#include "CitromAssert.h"
#include "Renderer/Renderer.h"

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

		// TODO: figure out whether do i really need to do this, because
		// ideally it should not need to be called
		#ifdef CT_EDITOR_ENABLED
		CT_CORE_FATAL("{}, {}", window->GetBackend()->GetWidth(), window->GetBackend()->GetHeight());
		::ImGui::GetIO().DisplaySize = ImVec2(window->GetBackend()->GetWidth(), window->GetBackend()->GetHeight());
		#endif

		m_Window->GetBackend()->ImGuiInitialize();
	}
	void ImGui::Terminate()
	{
		m_Window->GetBackend()->ImGuiTerminate();
	}

	void ImGui::NewFrame()
	{
		m_Window->GetBackend()->ImGuiNewFrame();
	}
}
#endif