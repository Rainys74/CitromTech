#include "EditorLayer.h"

#include "Profiling/Profiler.h"
#include "Logger/Logger.h"

#include "Editor/EditorConsole.h"
#include "Editor/ProfilerWindow.h"

#include "imgui.h"

using namespace Citrom;
using namespace EditorImGui;

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

    EditorConsole::ImGuiDraw();
    ProfilerWindow::ImGuiDraw();
}