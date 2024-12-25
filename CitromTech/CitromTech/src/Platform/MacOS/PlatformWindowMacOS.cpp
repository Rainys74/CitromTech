#ifdef CT_PLATFORM_MACOS
#include "Platform/PlatformWindow.h"

#include "Platform/Shared/PlatformWindowBackendGLFW.h"

namespace Citrom::Platform
{
        Window::Window()
            : m_Backend(nullptr)
        {
            m_Backend = new WindowBackendGLFW();
        }
        Window::~Window()
        {
            delete m_Backend;
        }

		void Window::Create(const int width, const int height, const CTL::String& title)
        {
            m_Backend->Create(width, height, title);
        }
        bool Window::WindowShouldClose() const
        {
            return m_Backend->WindowShouldClose();
        }
		void Window::PollEvents()
        {
            m_Backend->PollEvents();
        }
        
        void* Window::GLFWTryGetWnd()
        {
            return m_Backend->GLFWTryGetWnd();
        }
}
#endif