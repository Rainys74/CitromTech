#include "ImGuiLayer.h"
#include "ImGui/ImGuiStyles.h"

#include "Platform/PlatformImGui.h"
#include "Renderer/Renderer.h"

#include "Profiling/Profiler.h"

#include "imgui.h"

namespace Citrom
{
	static float g_ResizeFont = 0.0f;

	ImGuiLayer::ImGuiLayer()
	{
	}
	ImGuiLayer::~ImGuiLayer()
	{
	}

	/*
	void ImGuiLayer::RebuildFontsHint(float scale)
	{
		/*ImGuiIO& io = ImGui::GetIO();

		io.Fonts->Clear();

		constexpr float fontSize = 17.0f;

		io.Fonts->AddFontFromFileTTF("Editor/Assets/Fonts/Roboto/Roboto-Bold.ttf", fontSize * scale); // Black
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Editor/Assets/Fonts/Roboto/Roboto-Regular.ttf", fontSize * scale);* /

		constexpr float fontSize = 17.0f;

		g_ResizeFont = fontSize * scale;
	}
	*/

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

		ImGuiStyles::Push(ImGuiStyleObj{"ImGui Dark",		[]() {ImGui::StyleColorsDark();}	});
		ImGuiStyles::Push(ImGuiStyleObj{"ImGui Classic",	[]() {ImGui::StyleColorsClassic();} });
		ImGuiStyles::Push(ImGuiStyleObj{"ImGui Light",		[]() {ImGui::StyleColorsLight();}	});
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
		Renderer::ImGuiRenderDrawData(ImGui::GetDrawData());

		/*
		// Resize Fonts
		if (g_ResizeFont != 0.0f)
		{
			ImGuiIO& io = ImGui::GetIO();

			io.Fonts->Clear();

			io.Fonts->AddFontFromFileTTF("Editor/Assets/Fonts/Roboto/Roboto-Bold.ttf", g_ResizeFont); // Black
			io.FontDefault = io.Fonts->AddFontFromFileTTF("Editor/Assets/Fonts/Roboto/Roboto-Regular.ttf", g_ResizeFont);
			//CT_WARN("G_RESIZE_FONT! {}", g_ResizeFont);

			// Required for dynamic font loading
			//	io.Fonts->GetTexDataAsRGBA32();
			io.Fonts->Build();

			g_ResizeFont = 0.0f;
		}
		*/
	}
}