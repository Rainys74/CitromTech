#include "ProfilerWindow.h"
#include "Main/SharedMain.h"

#include "Profiling/Profiler.h"
#include "Profiling/ProfileResults.h"

#include "imgui.h"

using namespace Citrom;

namespace EditorImGui
{
    static void ProfilerStatsOverlay_ImGuiDraw()
    {
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        //ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);

        static bool showStatOverlay = false;
        ImGui::Begin("Statistics Overlay", &showStatOverlay, windowFlags);

        //ImGui::Text("FPS: %2.f", MainFPS());
        ImGui::Text("FPS: %.2f", MainFPS());
        ImGui::Text("DrawCalls: 69");
        
        //ImGui::Text("FB Scale: (%f, %f)", ImGui::GetIO().DisplayFramebufferScale.x, ImGui::GetIO().DisplayFramebufferScale.y); // MacOS Retina debugging
        //ImGui::Text("Display Size: (%f, %f)", ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);

        ImGui::End();
    }

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
                    //ImGui::Text("%s = %f ms", key, Profiler::ProfileResults::GetResults()[key] * 1000);
                    //ImGui::Text("%s = %f ms (%.2f%%)", key, Profiler::ProfileResults::GetResults()[key] * 1000, (Profiler::ProfileResults::GetResults()[key] / MainDeltaTime()) * 100);

                    float64 fpsWithoutFunction = 1.0 / (MainDeltaTime() - Profiler::ProfileResults::GetResults()[key]);

                    //ImGui::Text("%s = %f ms (%.1f FPS)", key, Profiler::ProfileResults::GetResults()[key] * 1000, MainFPS() - fpsWithoutFunction);
                    ImGui::Text("%s = %f ms (%.2f%%) | Impact: (%.1f FPS)", key, Profiler::ProfileResults::GetResults()[key] * 1000, (Profiler::ProfileResults::GetResults()[key] / MainDeltaTime()) * 100, MainFPS() - fpsWithoutFunction);
                }

                /*static std::unordered_map<std::string, std::vector<float>> history;
                for (const char* key : Profiler::ProfileResults::GetResultOrder())
                {
                    double timeInSeconds = Profiler::ProfileResults::GetResults()[key];
                    history[key].push_back(timeInSeconds * 1000); // Store ms
                    if (history[key].size() > 100) // Limit history size
                        history[key].erase(history[key].begin());

                    ImGui::Text("%s = %f ms (%.2f%%)", key, Profiler::ProfileResults::GetResults()[key] * 1000, (Profiler::ProfileResults::GetResults()[key] / MainDeltaTime()) * 100);
                    ImGui::PlotLines(("##sparkline_" + std::string(key)).c_str(), history[key].data(), history[key].size(), 0, nullptr, 0.0f, 10.0f, ImVec2(0, 40));
                }*/

                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::End();

        ProfilerStatsOverlay_ImGuiDraw();
	}
}
