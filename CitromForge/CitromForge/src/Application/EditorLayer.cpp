#include "EditorLayer.h"
#include "Main/SharedMain.h"

#include "Profiling/Profiler.h"
#include "Logger/Logger.h"

#include "Editor/EditorConsole.h"
#include "Editor/ProfilerWindow.h"
#include "Editor/PreferenceWindow.h"
#include "Editor/SceneHierarchy.h"
#include "Editor/InspectorWindow.h"
#include "Editor/ProjectSettingsWindow.h"
#include "Editor/RendererWindow.h"

#include "Editor/EditorCamera.h"
#include "Editor/EditorGizmos.h"

#include "CTL/CStringHandling.h"

#include "Platform/PlatformWindow.h"

#include "EntitySystem/SceneSerializer.h"
#include "NativeFileDialog/NativeFileDialog.h"

#include "imgui.h"
#include "ImGuizmo.h"

#include "ImGui/ImGuiToolkit.h"

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

void EditorLayer::OnUpdate(float64 deltaTime)
{
    EditorCamera::Get()->Update(deltaTime);
}

void EditorLayer::OnImGuiRender()
{
	//static bool show = true;
	//ImGui::ShowDemoWindow(&show);

    static bool preferenceWindowOpen = false; // preferenceWindowOn, Open, Show, Enabled, Status
    static bool sceneHierarchyOpen = true;

    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    bool openResolutionSetter = false;
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Scene", "Ctrl+N"))
            {
                GetCurrentScene()->ClearScene(); // TODO: you might want to move to shared main since it requires reconstructing the name of the scene and stuff
            }
            if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
            {
                NativeFileDialog nfd;
                NativeFileDialogFilter filters[] = { { "Citrom Tech Scene", "ctscene,cts,ctenv" } };
                std::string result = nfd.OpenFile(filters, CT_ARRAY_LENGTH(filters));
                if (result != "")
                {
                    Scene* scene = GetCurrentScene();
                    scene->ClearScene(); // To Reopen scene

                    SceneSerializer sceneSerializer(scene);
                    sceneSerializer.Deserialize(result);
                }
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Save", "Ctrl+S")) // Save Scene
            {
                std::string filePath("Assets/Scenes/");
                filePath.append(GetCurrentScene()->GetSceneName());
                filePath.append(".ctscene"); // .cts, .ctscene, .ctenv also .ctbs, .ctbinscene, .ctbscene, .ctscenebin

                SceneSerializer(GetCurrentScene()).Serialize(filePath);
            }
            if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
            {
                std::string filePath("Assets/Scenes/");
                filePath.append(GetCurrentScene()->GetSceneName()); // TODO: open nfd for save as
                filePath.append(".ctscene"); // .cts, .ctscene, .ctenv also .ctbs, .ctbinscene, .ctbscene, .ctscenebin

                SceneSerializer sceneSerializer(GetCurrentScene());
                sceneSerializer.Serialize(filePath);
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Quit")); // Exit

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            // This can also technically be put in "View" as well as maybe "Window"?
            ImGui::Separator();
            if (ImGui::BeginMenu("Set Display Mode"))
            {
                if (ImGui::MenuItem("Windowed"))
                {
                    GetMainWindow()->GetBackend()->SetDisplayMode(Platform::DisplayMode::Windowed);
                }
                if (ImGui::MenuItem("Borderless"))
                {
                    GetMainWindow()->GetBackend()->SetDisplayMode(Platform::DisplayMode::Borderless);
                }
                if (ImGui::MenuItem("Fullscreen"))
                {
                    GetMainWindow()->GetBackend()->SetDisplayMode(Platform::DisplayMode::Fullscreen);
                }

                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Set Resolution"))
                openResolutionSetter = true;

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

    if (openResolutionSetter)
        ImGui::OpenPopup("ResolutionSetterPopup");
    if (ImGui::BeginPopup("ResolutionSetterPopup"))
    {
        static Platform::Resolution currentResolution;
        static int width, height, refreshRate;
        if (openResolutionSetter)
        {
            currentResolution = GetMainWindow()->GetBackend()->GetResolution();

            width = currentResolution.width;
            height = currentResolution.height;
            refreshRate = currentResolution.refreshRate;
        }

        // TODO: maybe on same line do something like: (w) x (h) @ (rr)
        ImGui::InputInt("Width", &width);
        ImGui::InputInt("Height", &height);
        ImGui::InputInt("Refresh Rate", &refreshRate);
        ImGui::Text("NOTE: Resolution should not be set for Borderless windows."); // neither should refresh rate, actually refresh rate doesn't even do anything on windowed windows (including borderless).

        ImGui::Separator();

        constexpr float buttonWidth = 200.0f;
        if (ImToolkit::DrawCenteredButton("Apply", buttonWidth))
        {
            GetMainWindow()->GetBackend()->SetResolution(width, height, refreshRate);
            
            // TODO: probably don't copy/duplicate this code
            currentResolution = GetMainWindow()->GetBackend()->GetResolution();

            width = currentResolution.width;
            height = currentResolution.height;
            refreshRate = currentResolution.refreshRate;
        }

        ImGui::EndPopup();
    }

    //// Menu Bar
    //ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight())); // Position below the main menu bar
    //ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 50)); // Full width, fixed height
    //
    //ImGui::Begin("CustomMenuBar", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
    //ImGui::Text("This is a non-resizable window below the menu bar.");
    //ImGui::End();
    //
    //// Dock Space after Menu Bar
    //ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight() + 50));
    //ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y - ImGui::GetFrameHeight() - 50));
    //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    //ImGui::Begin("MainDockSpace", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground);
    //
    //// Set up docking
    //ImGui::DockSpace(ImGui::GetID("MainDockSpace"), ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);
    //
    //ImGui::End();
    //ImGui::PopStyleVar();

    //ImGui::Begin("##EditorMenuBar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
    //ImGui::Text("This is a non-resizable window below the menu bar.");
    //ImGui::End();

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

    ProjectSettingsWindow::ImGuiDraw();

    SceneHierarchyWindow::ImGuiDraw(nullptr);
    InspectorWindow::ImGuiDraw(nullptr);

    PreferenceWindow::ImGuiDraw(&preferenceWindowOpen);

    EditorConsole::ImGuiDraw();
    RendererWindow::ImGuiDraw();
    ProfilerWindow::ImGuiDraw();

    ImGuizmo::OPERATION gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
    EditorGizmos::ImGuiDraw(gizmoOperation); // TODO: Gizmos should NOT be drawn on top of everything.
}
