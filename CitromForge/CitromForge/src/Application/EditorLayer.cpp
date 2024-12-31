#include "EditorLayer.h"
#include "Main/SharedMain.h"

#include "Profiling/Profiler.h"
#include "Logger/Logger.h"

#include "imgui.h"

using namespace Citrom;

#define RESET       ImVec4(1.0f, 1.0f, 1.0f, 1.0f)   // White
#define RED         ImVec4(1.0f, 0.0f, 0.0f, 1.0f)   // Bright Red
#define GREEN       ImVec4(0.0f, 1.0f, 0.0f, 1.0f)   // Bright Green
#define YELLOW      ImVec4(1.0f, 1.0f, 0.0f, 1.0f)   // Bright Yellow
#define BLUE        ImVec4(0.0f, 0.0f, 1.0f, 1.0f)   // Bright Blue
#define PURPLE      ImVec4(0.5f, 0.0f, 0.5f, 1.0f)   // Bright Purple (Magenta)

#define DARKBLUE    ImVec4(0.0f, 0.0f, 0.5f, 1.0f)   // Dark Blue
#define LIGHTBLUE   ImVec4(0.0f, 0.5f, 1.0f, 1.0f)   // Cyan / Light Blue
#define GRAY        ImVec4(0.5f, 0.5f, 0.5f, 1.0f)   // Gray

constexpr uint32 MAX_LOG_ENTRIES = 1000;

struct LogEntry
{
    const char* log;
    ImVec4 color;
};
static CTL::DArray<LogEntry> g_ConsoleBuffer;

EditorLayer::EditorLayer()
{
    Logger::GetLogger()->PushCallback([](const char* log, LogColor logColor, Logger::LogLevel level)
    {
            LogEntry logEntry;

            size_t logLength = CTL::CString::GetLength(log) + 1;
            logEntry.log = new char[logLength];
            Memory::Copy((void*)logEntry.log, log, logLength);

            ImVec4 imColor;
            switch (static_cast<Platform::Console::TextColor>(logColor))
            {
                default: case Platform::Console::TextColor::Reset:  imColor = RESET; break;

                case Platform::Console::TextColor::Red:             imColor = RED; break;
                case Platform::Console::TextColor::Green:           imColor = GREEN; break;
                case Platform::Console::TextColor::Yellow:          imColor = YELLOW; break;
                case Platform::Console::TextColor::Blue:            imColor = BLUE; break;
                case Platform::Console::TextColor::Purple:          imColor = PURPLE; break;
            }
            logEntry.color = imColor;

            g_ConsoleBuffer.PushBack(logEntry);

            if (g_ConsoleBuffer.Count() >= MAX_LOG_ENTRIES)
            {
                delete[] g_ConsoleBuffer.begin()->log;
                g_ConsoleBuffer.Erase(g_ConsoleBuffer.begin()); // Remove the oldest log
            }
    });
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

    ImGui::Begin("Console");
    for (LogEntry& logEntry : g_ConsoleBuffer)
    {
        ImGui::TextColored(logEntry.color, logEntry.log);

        // If the user is at the bottom: scroll with the console
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        {
            ImGui::SetScrollHereY(1.0f);
        }
    }
    ImGui::End();

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