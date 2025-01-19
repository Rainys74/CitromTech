#pragma once

#include "Core.h"

class SceneHierarchyWindow
{
public:
	static void ImGuiDraw(bool* showWindow);

	static void* GetSelectedEntity();
};