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
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

		constexpr float fontSize = 17.0f; //13.0f;

		// Roboto
		io.Fonts->AddFontFromFileTTF("Editor/Assets/Fonts/Roboto/Roboto-Bold.ttf", fontSize); // Black
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Editor/Assets/Fonts/Roboto/Roboto-Regular.ttf", fontSize);
		// Inter
		//io.Fonts->AddFontFromFileTTF("Editor/Assets/Fonts/Inter/Inter_24pt-Bold.ttf", fontSize); // Black
		//io.FontDefault = io.Fonts->AddFontFromFileTTF("Editor/Assets/Fonts/Inter/Inter_24pt-Regular.ttf", fontSize);
		// Noto Sans CJK SC
		//io.Fonts->AddFontFromFileTTF("Editor/Assets/Fonts/Inter/Inter_24pt-Bold.ttf", fontSize); // Black
		//io.FontDefault = io.Fonts->AddFontFromFileTTF("Editor/Assets/Fonts/NotoSansCJKSC/NotoSansCJKsc-Regular.ttf", fontSize * 1.3f);

		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();
		//ImGui::StyleColorsLight();
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