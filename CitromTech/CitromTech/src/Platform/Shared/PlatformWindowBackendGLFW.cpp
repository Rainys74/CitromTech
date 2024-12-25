#include "PlatformWindowBackendGLFW.h"

#include "Logger/Logger.h"
#include "CitromAssert.h"

#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"

#ifdef CT_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
//#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3native.h>
#endif

namespace Citrom::Platform
{
    static void WindowCallbackProcedure(GLFWwindow* glfwWindow)
    {
        // WM_CLOSE
        glfwSetWindowCloseCallback(glfwWindow, [](GLFWwindow* window)
        {
            //WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            WindowCloseEvent windowCloseEvent;
			windowCloseEvent.exitCode = 69;

			EventBus::GetDispatcher<WindowEvents>()->Dispatch(windowCloseEvent);

            // Should close the window
        });
        // WM_SIZE
        glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow* window, int width, int height)
        {
            WindowResizeEvent windowResizeEvent;
            windowResizeEvent.width = width;
            windowResizeEvent.height = height;

            EventBus::GetDispatcher<WindowEvents>()->Dispatch(windowResizeEvent);
        });
        // WM_MOVE
        glfwSetWindowPosCallback(glfwWindow, [](GLFWwindow* window, int xPos, int yPos) 
        {
            WindowMoveEvent windowMoveEvent;
            windowMoveEvent.x = xPos;
            windowMoveEvent.y = yPos;

            EventBus::GetDispatcher<WindowEvents>()->Dispatch(windowMoveEvent);
        });
        // WM_LBUTTONDOWN, WM_LBUTTONUP, WM_RBUTTONDOWN, WM_RBUTTONUP, WM_MBUTTONDOWN, WM_MBUTTONUP, WM_XBUTTONDOWN, WM_XBUTTONUP
        glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* window, int button, int action, int mods)
        {
            switch (action)
            {
                CT_CORE_TRACE("{}", button);
                case GLFW_PRESS:
                {
                    MouseDownEvent mouseDownEvent;
                    int transformedButton = button + 1;

                    if (transformedButton >= 1 && transformedButton <= 20)
                        mouseDownEvent.mouseButton = static_cast<EventMouseButton>(transformedButton);
                    else
                        mouseDownEvent.mouseButton = EventMouseButton::Null;

                    EventBus::GetDispatcher<MouseEvents>()->Dispatch(mouseDownEvent);
                }
                break;
                case GLFW_RELEASE:
                {
                    MouseUpEvent mouseUpEvent;
                    int transformedButton = button + 1;

                    if (transformedButton >= 1 && transformedButton <= 20)
                        mouseUpEvent.mouseButton = static_cast<EventMouseButton>(transformedButton);
                    else
                        mouseUpEvent.mouseButton = EventMouseButton::Null;

                    EventBus::GetDispatcher<MouseEvents>()->Dispatch(mouseUpEvent);
                }
                break;
            }
        });
    }

    WindowBackendGLFW::WindowBackendGLFW()
        : m_WindowShouldClose(false), m_Width(0), m_Height(0), m_Window(nullptr)
    {
        // TODO: this should only be initialized once in the entire program
        CT_CORE_VERIFY(glfwInit(), "Failed to initialize GLFW!");
    }
    WindowBackendGLFW::~WindowBackendGLFW()
    {
        glfwDestroyWindow(m_Window);

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
        WindowCallbackProcedure(m_Window);
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
    void* WindowBackendGLFW::GLFWTryGetWnd()
    {
        return static_cast<void*>(m_Window);
    }
    #ifdef CT_PLATFORM_WINDOWS
    void* WindowBackendGLFW::Win32TryGetHWnd()
    {
        return static_cast<void*>(glfwGetWin32Window(m_Window));
    }
    #endif
}