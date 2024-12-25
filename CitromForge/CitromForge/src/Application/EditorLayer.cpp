#include "EditorLayer.h"

//#include "imgui.h"
//#include "backends/imgui_impl_glfw.h"
//#include "backends/imgui_impl_opengl3.h"
//
//#ifdef CT_PLATFORM_WINDOWS
//#include "backends/imgui_impl_dx11.h"
//#endif

static void InitializeImGuiWindow()
{
	// if glfw & opengl
	//ImGui_ImplGlfw_InitForOpenGL(window, true); // instead of ImGui_ImplGlfwGL3_Init()
	//ImGui_ImplOpenGL3_Init("#version 460");

	// if win32 & opengl
	//CT_ASSERT(ImGui_ImplWin32_InitForOpenGL(window->Win32TryGetHWND()), "Failed to Initialize ImGui Win32 implementation for OpenGL.");
	//CT_ASSERT(ImGui_ImplOpenGL3_Init(), "Failed to Initialize ImGui OpenGL 3 implementation.");

	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
}

EditorLayer::EditorLayer()
{
}

EditorLayer::~EditorLayer()
{
}

void EditorLayer::OnAttach()
{
	//ImGui::CreateContext();
	//ImGui::StyleColorsDark();

	//ImGuiIO& io = ImGui::GetIO();
	//io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	//io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;


}

void EditorLayer::OnDetach()
{
	//ImGui_ImplOpenGL3_Shutdown();
	//ImGui_ImplWin32_Shutdown();
	//ImGui::DestroyContext();
}

void EditorLayer::OnUpdate()
{

	//ImGui::NewFrame();

	//static bool show = true;
	//ImGui::ShowDemoWindow(&show);

	//ImGui::Render();

}

void EditorLayer::OnImGuiRender()
{

}