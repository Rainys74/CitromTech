#pragma once

#ifdef CT_PLATFORM_WINDOWS
#include "Platform/PlatformWindow.h"

#include "WindowsIncludes.h"

#include "Events/EventSystem.h"

namespace Citrom::Platform
{
    class WindowBackendWin32 : public WindowBackend
    {
    public:
        WindowBackendWin32();
        virtual ~WindowBackendWin32();

        virtual void Create(const int width, const int height, const CTL::String& title) override;
        virtual bool WindowShouldClose() const override;
        virtual void PollEvents() override;
    private:
        bool m_WindowShouldClose;
        int m_Width, m_Height;
        EventBus m_EventBus;

        // Win32
        TCHAR* m_ClassName; // LPCWSTR
        HINSTANCE m_HInstance;
        HWND m_HWnd;

        // OpenGL
        //HDC m_DC;
        //HGLRC m_RC;
        //int m_PF;
    };
}
#endif