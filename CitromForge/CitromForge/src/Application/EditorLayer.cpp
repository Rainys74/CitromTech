#include "EditorLayer.h"

#include "Profiling/Profiler.h"
#include "Logger/Logger.h"

#include "Editor/EditorConsole.h"
#include "Editor/ProfilerWindow.h"
#include "Editor/PreferenceWindow.h"
#include "Editor/SceneHierarchy.h"

#include "CTL/CStringHandling.h"

#include "imgui.h"

using namespace Citrom;
using namespace EditorImGui;

bool g_ShowAboutWindow;

EditorLayer::EditorLayer()
{
    EditorConsole::Initialize();
}

EditorLayer::~EditorLayer()
{
}

void EditorLayer::OnAttach()
{

}

void EditorLayer::OnDetach()
{
    //EditorConsole::Destroy();
}

void EditorLayer::OnUpdate()
{
}

void EditorLayer::OnImGuiRender()
{
	//static bool show = true;
	//ImGui::ShowDemoWindow(&show);

    static bool preferenceWindowOpen = false; // preferenceWindowOn, Open, Show, Enabled, Status
    static bool sceneHierarchyOpen = true;

    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Options"))
        {
            if (ImGui::MenuItem("Test...")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Editor Preferences..."))
            {
                preferenceWindowOpen = true;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About"))
            {
                g_ShowAboutWindow = true;
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (g_ShowAboutWindow)
    {
        constexpr const char* aboutTextTest =
            "This is an example of a scrollable, read-only text box.\n"
            "You can add a large pile of text here and it will be displayed as a scrolling area.\n\n"
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
            "Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
            "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris "
            "nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit "
            "in voluptate velit esse cillum dolore eu fugiat nulla pariatur.\n\n"
            "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n\n"
            "Further details can be added here. Enjoy using this application!";

        ImGui::Begin("About Citrom Tech", &g_ShowAboutWindow);

        //ImVec2 textBoxSize = ImVec2(400, 300);
        //ImVec2 windowSize = ImGui::GetWindowSize();
        //ImGui::SetCursorPos(ImVec2((windowSize.x - textBoxSize.x) / 2.0f, (windowSize.y - textBoxSize.y) / 2.0f));

        //ImGui::InputTextMultiline("##AboutTextBox", (char*)aboutTextTest, CTL::CString::GetLength(aboutTextTest)+1, ImVec2(400, 300), ImGuiInputTextFlags_ReadOnly);

        ImGui::BeginChild("TextContainer", ImVec2(400, 300), true, ImGuiWindowFlags_NoScrollWithMouse);
        ImGui::TextWrapped("%s", aboutTextTest);
        ImGui::EndChild();

        ImGui::End();
    }

    ImGui::Begin("Lighting"); ImGui::End();

    ImGui::Begin("Project Settings"); 
    if (ImGui::BeginTabBar("ProjectSettingTabs"))
    {
        if (ImGui::BeginTabItem("Graphics"))
        {
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Input System"))
        {
            ImGui::EndTabItem();
        } 
        if (ImGui::BeginTabItem("Input System2")) ImGui::EndTabItem();

        ImGui::EndTabBar();
    }
    ImGui::End();

    SceneHierarchyWindow::ImGuiDraw(nullptr);

    PreferenceWindow::ImGuiDraw(&preferenceWindowOpen);

    EditorConsole::ImGuiDraw();
    ProfilerWindow::ImGuiDraw();
}