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

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

		//io.DisplaySize = ImVec2(1280, 720); // Fixes errors, but ideally should not need to be called

		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();
		//ImGui::StyleColorsLight();

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
		Renderer::ImGuiInit();
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