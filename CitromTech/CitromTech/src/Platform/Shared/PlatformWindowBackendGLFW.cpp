#include "PlatformWindowBackendGLFW.h"

#include "Logger/Logger.h"
#include "CitromAssert.h"

namespace Citrom::Platform
{
    WindowBackendGLFW::WindowBackendGLFW()
        : m_WindowShouldClose(false), m_Width(0), m_Height(0), m_Window(nullptr)
    {
        CT_CORE_ASSERT(glfwInit(), "Failed to initialize GLFW!");
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
        glfwSwapInterval(1);
    
        // init glew/glad (assert glewInit() == GLEW_OK)
    }
    bool WindowBackendGLFW::WindowShouldClose() const
    {
        return glfwWindowShouldClose(m_Window);
    }
    void WindowBackendGLFW::PollEvents()
    {
        // TODO: move contexts over to a different platform implementation similar to Torque3D
        glfwSwapBuffers(m_Window);
    
        glfwPollEvents();
    }
}