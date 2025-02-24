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

        virtual void SetDisplayMode(DisplayMode displayMode, const uint32 refreshRate = 0) override;
        virtual void SetResolution(const uint32 width, const uint32 height, const uint32 refreshRate = 0, const int xPos = 0, const int yPos = 0) override;
        virtual Resolution GetResolution() override;

        virtual int GetWidth() override;
        virtual int GetHeight() override;

        virtual void* Win32TryGetHWnd() override;

        // ImGui
        virtual void ImGuiInitialize() override;
        virtual void ImGuiTerminate() override;

        virtual const char* GetName() override { return "Win32"; }
    private:
        bool m_WindowShouldClose;
        int m_Width, m_Height;
        EventBus m_EventBus;

        // Win32
        TCHAR* m_ClassName; // LPCWSTR
        HINSTANCE m_HInstance;
        HWND m_HWnd;
    };
}
#endif