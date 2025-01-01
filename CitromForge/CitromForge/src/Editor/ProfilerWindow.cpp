#include "ProfilerWindow.h"
#include "Main/SharedMain.h"

#include "Profiling/Profiler.h"
#include "Profiling/ProfileResults.h"

#include "imgui.h"

using namespace Citrom;

namespace EditorImGui
{
	void ProfilerWindow::ImGuiDraw()
	{
        ImGui::Begin("Profiler");

        if (ImGui::BeginTabBar("ProfilerTabs"))
        {
            if (ImGui::BeginTabItem("Statistics"))
            {
                if (ImGui::CollapsingHeader("Graphics:", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::Text("%.1f FPS (%.2f ms)", MainFPS(), MainDeltaTime() * 1000);
                    ImGui::Spacing();
                    ImGui::Text("Main Thread: %.2f ms (%.1f FPS)", MainDeltaTime() * 1000, MainFPS());
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
}