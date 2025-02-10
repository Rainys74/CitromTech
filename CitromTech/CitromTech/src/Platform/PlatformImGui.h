#pragma once

#include "Core.h"

#include "PlatformWindow.h"

namespace Citrom::Platform
{
	class ImGui
	{
	public:
		static void Initialize(Window* window);
		static void Terminate();
	private:
		static Window* m_Window;
	};
}