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

        virtual void* GLFWTryGetWnd() override;
        
        IF_WINDOWS(virtual void* Win32TryGetHWnd() override);
    private:
        bool m_WindowShouldClose;
        int m_Width, m_Height;

        GLFWwindow* m_Window;
    };
}