#include "PlatformWindowBackendGLFW.h"

#include "Logger/Logger.h"

#include "GLFW/glfw3.h"

namespace Citrom::Platform
{
        WindowBackendGLFW::WindowBackendGLFW()
            : m_WindowShouldClose(false), m_Width(0), m_Height(0)
        {
        }
		WindowBackendGLFW::~WindowBackendGLFW()
        {

        }

		void WindowBackendGLFW::Create(const int width, const int height, const CTL::String& title)
        {
            CT_TRACE("Test: {}, {}, {}", width, height, title.CStr());
        }
        bool WindowBackendGLFW::WindowShouldClose() const
        {
            return 0;
        }
		void WindowBackendGLFW::PollEvents()
        {

        }
}