#ifdef CT_PLATFORM_WINDOWS
#include "Platform/PlatformWindow.h"

//#include "Platform/Shared/PlatformWindowBackendGLFW.h"
#include "Platform/Windows/PlatformWindowBackendWin32.h"

#include "Profiling/Profiler.h"

namespace Citrom::Platform
{
    Window::Window()
        : m_Backend(nullptr)
    {
        m_Backend = new WindowBackendWin32(); // WindowBackendGLFW, WindowBackendWin32
    }
    Window::~Window()
    {
        delete m_Backend;
    }

    void Window::Create(const int width, const int height, const CTL::String& title)
    {
        CT_PROFILE_MEMBER_FUNCTION();
        //PROFILE_STATIC_FUNCTION(Window);
        //PROFILE_GLOBAL_FUNCTION();
        m_Backend->Create(width, height, title);
    }
    bool Window::WindowShouldClose() const
    {
        return m_Backend->WindowShouldClose();
    }
    void Window::PollEvents()
    {
        CT_PROFILE_MEMBER_FUNCTION();

        //Platform::Utils::Sleep(1240);
        size_t j = 0;
        for (size_t i = 0; i < 1000000000; i++)
        {
            j += i;
        }
        m_Backend->PollEvents();
    }
    void* Window::Win32TryGetHWnd()
    {
        return m_Backend->Win32TryGetHWnd();
    }
}
#endif