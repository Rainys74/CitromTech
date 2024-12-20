#include "PlatformWindowBackendGLFW.h"

#include "Logger/Logger.h"
#include "CitromAssert.h"

#ifdef CT_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
//#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3native.h>
#endif

namespace Citrom::Platform
{
    static void WindowCallbackProcedure(const WindowBackendGLFW* pWindow)
    {
        //glfwSetWindowCloseCallback(pWindow, [](GLFWwindow* window)
        //{
        //    //WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        //
        //});
    }

    WindowBackendGLFW::WindowBackendGLFW()
        : m_WindowShouldClose(false), m_Width(0), m_Height(0), m_Window(nullptr)
    {
        // TODO: this should only be initialized once in the entire program
        CT_CORE_VERIFY(glfwInit(), "Failed to initialize GLFW!");
    }
    WindowBackendGLFW::~WindowBackendGLFW()
    {
        glfwTerminate();
    }
    
    void WindowBackendGLFW::Create(const int width, const int height, const CTL::String& title)
    {
        CT_TRACE("Test: {}, {}, {}", width, height, title.CStr());
    
        // TODO: move contexts over to a different platform implementation similar to Torque3D
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    
        m_Window = glfwCreateWindow(width, height, title.CStr(), NULL, NULL);
        CT_CORE_ASSERT(m_Window, "Failed to create a GLFW Window!");
    
        // TODO: move contexts over to a different platform implementation similar to Torque3D
        glfwMakeContextCurrent(m_Window);
        //glfwSetWindowUserPointer(m_Window, &m_Data);
        glfwSwapInterval(1);
    
        // init glew/glad (assert glewInit() == GLEW_OK)

        // Handle window event callbacks
        WindowCallbackProcedure(this);
    }
    bool WindowBackendGLFW::WindowShouldClose() const
    {
        return glfwWindowShouldClose(m_Window);
    }
    void WindowBackendGLFW::PollEvents()
    {
        glfwPollEvents();

        // TODO: move contexts over to a different platform implementation similar to Torque3D
        glfwSwapBuffers(m_Window);
    }
    #ifdef CT_PLATFORM_WINDOWS
    void* WindowBackendGLFW::Win32TryGetHWnd()
    {
        return static_cast<void*>(glfwGetWin32Window(m_Window));
    }
    #endif
}