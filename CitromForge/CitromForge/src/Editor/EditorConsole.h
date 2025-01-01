#pragma once

#include "CTL/DArray.h"

struct LogEntry;
class EditorConsole
{
public:
	static void Initialize();
	static void Destroy();

	static void ImGuiDraw();
private:
	static CTL::DArray<LogEntry> s_ConsoleBuffer;
};