#ifdef CT_PLATFORM_WINDOWS
#include "Platform/PlatformWindow.h"
#include "ArgumentHandler.h"

#include "Platform/Shared/PlatformWindowBackendGLFW.h"
#include "Platform/Windows/PlatformWindowBackendWin32.h"

#include "Profiling/Profiler.h"

namespace Citrom::Platform
{
    Window::Window()
        : m_Backend(nullptr)
    {
        //if (ArgumentHandler::HasArgument("-force-glfw"))
        //    m_Backend = new WindowBackendGLFW();
        //else
            m_Backend = new WindowBackendWin32(); // WindowBackendGLFW, WindowBackendWin32
    }
    Window::~Window()
    {
        delete m_Backend;
    }

    void Window::Create(const int width, const int height, const CTL::String& title)
    {
        CT_PROFILE_MEMBER_FUNCTION();

        if (ArgumentHandler::HasArgument("-force-glfw"))
        {
            delete m_Backend;
            m_Backend = new WindowBackendGLFW();
        }

        m_Backend->Create(width, height, title);
    }
    bool Window::WindowShouldClose() const
    {
        return m_Backend->WindowShouldClose();
    }
    void Window::PollEvents()
    {
        CT_PROFILE_MEMBER_FUNCTION();

        m_Backend->PollEvents();
    }

    float Window::GetBackingScaleFactor()
    {
        HWND hWnd = (HWND)m_Backend->Win32TryGetHWnd();

        //HDC hdc = GetDC(hWnd);
        //UINT dpi = GetDeviceCaps(hdc, LOGPIXELSX);
        ////UINT dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
        //ReleaseDC(hWnd, hdc);
        UINT dpi = GetDpiForWindow(hWnd);

        float scaleFactor = dpi / CCast<float32>(USER_DEFAULT_SCREEN_DPI); // 96.0f DPI is the default DPI for 1.0 scaling
        return scaleFactor;
    }

    void* Window::Win32TryGetHWnd()
    {
        return m_Backend->Win32TryGetHWnd();
    }
    void* Window::CocoaTryGetNSWnd()
    {
        return nullptr;
    }
    void* Window::GLFWTryGetWnd()
    {
        return m_Backend->GLFWTryGetWnd();
    }
}
#endif
