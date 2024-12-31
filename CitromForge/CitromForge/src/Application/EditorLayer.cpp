#include "EditorLayer.h"

#include "Profiling/Profiler.h"

#include "imgui.h"

using namespace Citrom;

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
	//static bool show = true;
	//ImGui::ShowDemoWindow(&show);

	ImGui::Begin("Profiler");
	//Profiler::ProfileResults::IterateResultsCallback([](const char* key, const float64 time)
	//{
	//	ImGui::Text("%s = %f", key, time);
	//});

	for (const std::string& key : Profiler::ProfileResults::GetResultOrder())
	{
		ImGui::Text("%s = %f s", key.c_str(), Profiler::ProfileResults::GetResults()[key]);
	}

	ImGui::End();
}