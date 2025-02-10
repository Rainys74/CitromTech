#include "Platform/PlatformImGui.h"

#include "CitromAssert.h"
#include "Renderer/Renderer.h"

#include "imgui.h"

namespace Citrom::Platform
{
	Window* ImGui::m_Window;

	void ImGui::Initialize(Window* window)
	{
		m_Window = window;

		// TODO: figure out whether do i really need to do this, because
		// ideally it should not need to be called
		#ifdef CT_EDITOR_ENABLED
		::ImGui::GetIO().DisplaySize = ImVec2(window->GetBackend()->GetWidth(), window->GetBackend()->GetHeight());
		#endif

		m_Window->GetBackend()->ImGuiInitialize();
	}
	void ImGui::Terminate()
	{
		m_Window->GetBackend()->ImGuiTerminate();
	}
}