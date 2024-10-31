#ifdef CT_PLATFORM_WINDOWS
#include "PlatformWindowWindows.h"

#include "Platform/Windows/PlatformWindowBackendWin32.h"

namespace Citrom::Platform
{
    WindowsWindow::WindowsWindow()
        : m_Backend(nullptr)
    {
        m_Backend = new WindowBackendWin32();
    }
    WindowsWindow::~WindowsWindow()
    {
        delete m_Backend;
    }

    void WindowsWindow::Create(const int width, const int height, const CTL::String& title)
    {
        m_Backend->Create(width, height, title);
    }
    bool WindowsWindow::WindowShouldClose() const
    {
        return m_Backend->WindowShouldClose();
    }
    void WindowsWindow::PollEvents()
    {
        m_Backend->PollEvents();
    }
}
#endif