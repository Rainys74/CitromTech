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

		static void NewFrame();
		
		//static void RenderDrawData(...);
	private:
		static Window* m_Window;
	};
}