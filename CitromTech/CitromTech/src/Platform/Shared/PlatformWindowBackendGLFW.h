#pragma once

#include "Platform/PlatformWindow.h"

#include "GLFW/glfw3.h"

namespace Citrom::Platform
{
    class WindowBackendGLFW : public WindowBackend
    {
    public:
        WindowBackendGLFW();
		virtual ~WindowBackendGLFW();

        virtual void Create(const int width, const int height, const CTL::String& title) override;
        virtual bool WindowShouldClose() const override;
        virtual void PollEvents() override;

        virtual void SetDisplayMode(DisplayMode displayMode, const uint32 refreshRate = 0) override;
        virtual void SetResolution(const uint32 width, const uint32 height, const uint32 refreshRate = 0, const int xPos = 0, const int yPos = 0) override;
        virtual Resolution GetResolution() override;

        virtual int GetWidth() override;
        virtual int GetHeight() override;

        virtual void* GLFWTryGetWnd() override;
        
        IF_WINDOWS(virtual void* Win32TryGetHWnd() override);
        IF_MACOS(virtual void* CocoaTryGetNSWnd() override);
        IF_MACOS(virtual void* CocoaTryGetNSView() override);

        // ImGui
        virtual void ImGuiInitialize() override;
        virtual void ImGuiTerminate() override;

        virtual const char* GetName() override { return "GLFW"; }
    private:
        bool m_WindowShouldClose;
        int m_Width, m_Height;

        GLFWwindow* m_Window;
    };
}
