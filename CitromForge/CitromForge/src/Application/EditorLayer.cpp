#include "EditorLayer.h"

#include "Profiling/Profiler.h"
#include "Logger/Logger.h"

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
    Logger::GetLogger()->PushCallback([](const char* log, LogColor logColor, Logger::LogLevel level)
    {
    });
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

    if (ImGui::BeginTabBar("ProfilerTabs"))
    {
        if (ImGui::BeginTabItem("Statistics"))
        {
            if (ImGui::CollapsingHeader("Graphics:", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Text("%.1f FPS (%.2f ms)", 45.7f, 3.7254f);
                ImGui::Spacing();
                ImGui::Text("Main Thread: %.2f ms (%.1f FPS)", 3.7254f, 45.7f);
                ImGui::Text("Render Thread: %.2f ms (%.1f FPS)", 2.3457f, 69.420f);
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Functions"))
        {
            for (const char* key : Profiler::ProfileResults::GetResultOrder())
            {
                // TODO: maybe make it so if time is < 1.0 it shows up as ms, but if >=1.0 shows up as seconds
                ImGui::Text("%s = %f ms", key, Profiler::ProfileResults::GetResults()[key] * 1000);
            }
            ImGui::EndTabItem();
        }
	    ImGui::EndTabBar();
    }

	ImGui::End();
}