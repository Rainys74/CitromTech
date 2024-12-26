#include "EditorLayer.h"

#include "imgui.h"

EditorLayer::EditorLayer()
{
}

EditorLayer::~EditorLayer()
{
}

void EditorLayer::OnAttach()
{
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate()
{
}

void EditorLayer::OnImGuiRender()
{
	static bool show = true;
	ImGui::ShowDemoWindow(&show);
}