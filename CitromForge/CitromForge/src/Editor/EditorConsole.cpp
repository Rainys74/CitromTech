#include "EditorConsole.h"

#include "Profiling/Profiler.h"
#include "Profiling/ProfileResults.h"
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

namespace EditorImGui
{
    constexpr uint32 MAX_LOG_ENTRIES = 1000;

    struct LogEntry
    {
        const char* log;
        ImVec4 color;
    };

    CTL::DArray<LogEntry> EditorConsole::s_ConsoleBuffer;

    void EditorConsole::Initialize()
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

                s_ConsoleBuffer.PushBack(logEntry);

                if (s_ConsoleBuffer.Count() >= MAX_LOG_ENTRIES)
                {
                    delete[] s_ConsoleBuffer.begin()->log;
                    s_ConsoleBuffer.Erase(s_ConsoleBuffer.begin()); // Remove the oldest log
                }
            });
    }

    void EditorConsole::Destroy()
    {
        for (uint32 i = 0; i < s_ConsoleBuffer.Count(); i++)
            delete[] s_ConsoleBuffer[i].log;

        s_ConsoleBuffer.Clear();
    }

    void EditorConsole::ImGuiDraw()
    {
        ImGui::Begin("Console");
        for (LogEntry& logEntry : s_ConsoleBuffer)
        {
            ImGui::TextColored(logEntry.color, logEntry.log);

            // If the user is at the bottom: scroll with the console
            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            {
                ImGui::SetScrollHereY(1.0f);
            }
        }
        ImGui::End();
    }
}