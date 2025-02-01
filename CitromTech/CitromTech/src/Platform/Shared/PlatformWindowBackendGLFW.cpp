#include "PlatformWindowBackendGLFW.h"

#include "Logger/Logger.h"
#include "CitromAssert.h"

#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"

#include "Input/KeyboardInput.h"

#ifdef CT_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
//#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3native.h>
#endif

// ImGui
#ifdef CT_EDITOR_ENABLED
#include "CitromAssert.h"
#include "Renderer/Renderer.h"

#include "imgui.h"

#include "backends/imgui_impl_glfw.h"

#include "backends/imgui_impl_opengl3.h"

#ifdef CT_PLATFORM_WINDOWS
#include "backends/imgui_impl_dx11.h"
#elif defined(CT_PLATFORM_MACOS)
//#include "backends/imgui_impl_metal.h"
#endif

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

            glfwSetWindowShouldClose(window, GLFW_TRUE);
            //glfwDestroyWindow(window);
            //glfwTerminate();
        });
        // WM_SIZE
        glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow* window, int width, int height)
        {
            WindowResizeEvent windowResizeEvent;
            windowResizeEvent.width = width;
            windowResizeEvent.height = height;

            EventBus::GetDispatcher<WindowEvents>()->Dispatch(windowResizeEvent);

            // TODO: figure out whether do i really need to do this, because
            // ideally it should not need to be called
            #ifdef CT_EDITOR_ENABLED
            ImGui::GetIO().DisplaySize = ImVec2(width, height);
            #endif
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
        // WM_MOUSEMOVE
        glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* window, double xpos, double ypos)
        {
            MouseMoveEvent mouseMoveEvent;
            mouseMoveEvent.x = xpos;
            mouseMoveEvent.y = ypos;

            EventBus::GetDispatcher<MouseEvents>()->Dispatch(mouseMoveEvent);
        });
        // WM_MOUSEWHEEL, WM_MOUSEHWHEEL
        glfwSetScrollCallback(glfwWindow, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            MouseScrollEvent mouseScrollEvent;
            mouseScrollEvent.x = xoffset;
            mouseScrollEvent.y = yoffset;

            EventBus::GetDispatcher<MouseEvents>()->Dispatch(mouseScrollEvent);
        });
        // WM_KEYDOWN, WM_KEYUP
        glfwSetKeyCallback(glfwWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyDownEvent keyDownEvent;
                    keyDownEvent.keyCode = Input::GLFWKeyToInputSystem(key);

                    EventBus::GetDispatcher<KeyEvents>()->Dispatch(keyDownEvent);
                }
                break;
                case GLFW_RELEASE:
                {
                    KeyUpEvent keyUpEvent;
                    keyUpEvent.keyCode = Input::GLFWKeyToInputSystem(key);

                    EventBus::GetDispatcher<KeyEvents>()->Dispatch(keyUpEvent);
                }
                break;
                case GLFW_REPEAT:
                {
                    KeyRepeatEvent keyRepeatEvent;
                    keyRepeatEvent.keyCode = Input::GLFWKeyToInputSystem(key);

                    EventBus::GetDispatcher<KeyEvents>()->Dispatch(keyRepeatEvent);
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

        // Fire a window resize event to match Win32 // TODO: it's actually fired on the first UpdateWindow() call in win32..
        WindowResizeEvent windowResizeEvent;
        windowResizeEvent.width = width;
        windowResizeEvent.height = height;

        EventBus::GetDispatcher<WindowEvents>()->Dispatch(windowResizeEvent);
    
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

    int WindowBackendGLFW::GetWidth()
    {
        glfwGetWindowSize(m_Window, &m_Width, &m_Height);
        return m_Width;
    }
    int WindowBackendGLFW::GetHeight()
    {
        glfwGetWindowSize(m_Window, &m_Width, &m_Height);
        return m_Height;
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

    // ImGui
    #ifdef CT_EDITOR_ENABLED
    void WindowBackendGLFW::ImGuiInitialize()
    {
        if (RenderAPI::GraphicsAPIManager::IsGraphicsAPI(RenderAPI::GraphicsAPI::OpenGL))
        {
            CT_CORE_VERIFY(ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)this->GLFWTryGetWnd(), true), "Failed to Initialize ImGui GLFW implementation for OpenGL.");
            CT_CORE_VERIFY(ImGui_ImplOpenGL3_Init("#version 410"), "Failed to Initialize ImGui OpenGL 4.1 implementation.");
        }
#ifdef CT_PLATFORM_WINDOWS
        else if (RenderAPI::GraphicsAPIManager::IsGraphicsAPI(RenderAPI::GraphicsAPI::DirectX11))
        {
            CT_CORE_VERIFY(ImGui_ImplGlfw_InitForOther((GLFWwindow*)this->GLFWTryGetWnd(), true), "Failed to initialize ImGui GLFW implementation.");
        }
#endif
    }
    void WindowBackendGLFW::ImGuiTerminate()
    {
        if (RenderAPI::GraphicsAPIManager::IsGraphicsAPI(RenderAPI::GraphicsAPI::OpenGL))
        {
            ImGui_ImplOpenGL3_Shutdown();
        }
#ifdef CT_PLATFORM_WINDOWS
        else if (RenderAPI::GraphicsAPIManager::IsGraphicsAPI(RenderAPI::GraphicsAPI::DirectX11))
        {
            ImGui_ImplDX11_Shutdown();
        }
#endif

        ImGui_ImplGlfw_Shutdown();
    }
    void WindowBackendGLFW::ImGuiNewFrame()
    {
        if (RenderAPI::GraphicsAPIManager::IsGraphicsAPI(RenderAPI::GraphicsAPI::OpenGL))
        {
            ImGui_ImplOpenGL3_NewFrame();
        }
#ifdef CT_PLATFORM_WINDOWS
        else if (RenderAPI::GraphicsAPIManager::IsGraphicsAPI(RenderAPI::GraphicsAPI::DirectX11))
        {
            ImGui_ImplDX11_NewFrame();
        }
#endif
    }
    #else
    void WindowBackendGLFW::ImGuiInitialize() {}
    void WindowBackendGLFW::ImGuiTerminate() {}
    void WindowBackendGLFW::ImGuiNewFrame() {}
    #endif
}