#pragma once

#include "Core.h"

class SceneHierarchyWindow
{
public:
	static void ImGuiDraw(bool* showWindow);

	// TODO: use event callbacks (onSelectEntity) like Hazel or at least include a way to tell SHP that you selected an entity for example by mouse-picking
	static uint32 GetSelectedEntity();
};