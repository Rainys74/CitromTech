#include "ImGuiLayer.h"

#include "Platform/PlatformImGui.h"
#include "Renderer/Renderer.h"

#include "Profiling/Profiler.h"

#include "imgui.h"

namespace Citrom
{
	ImGuiLayer::ImGuiLayer()
	{
	}
	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

		//ImGuiLayer::Initialize();
	}
	void ImGuiLayer::OnDetach()
	{
		CT_PROFILE_MEMBER_FUNCTION();

		Platform::ImGui::Terminate();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Initialize(Platform::Window* window)
	{
		Platform::ImGui::Initialize(window);
	}

	void ImGuiLayer::Begin()
	{
		Platform::ImGui::NewFrame();
		ImGui::NewFrame();
	}
	void ImGuiLayer::End()
	{
		CT_PROFILE_MEMBER_FUNCTION();

		ImGui::Render();
		Renderer::ImGuiRenderDrawData(); // TODO: maybe make it so you need to pass in ImGui draw data?
	}
}