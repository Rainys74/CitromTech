#pragma once

#include "Core.h"
#include "Platform/PlatformWindow.h"

#include "CTL/ScopedPointer.h"

namespace Citrom::Platform::GL
{
	// create context
	// swap buffers/present
	// destroy context

	/*void init();

	void* CreateContextGL(PlatformWindow* window);

	void MakeCurrentGL(PlatformWindow* window, void* glContext);

	void setVSync(const int i);*/

	struct ContextGLData;
	class ContextGL
	{
	public:
		ContextGL(Window* window);
		~ContextGL();

		void Create();
		void SwapBuffers();

		void SetVSync(const int vSync);
	private:
		CTL::ScopedPtr<ContextGLData> m_Internal;
		Window* m_Window;
	};
}