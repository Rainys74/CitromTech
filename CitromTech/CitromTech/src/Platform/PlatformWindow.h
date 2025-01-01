#pragma once

#include "CTL/String.h"

/*
* Responsible for the creation of a Surface for
* rendering and handling user interactions.
*/
namespace Citrom::Platform
{
    class WindowBackend
    {
    public:
        WindowBackend()
            : m_WindowShouldClose(false), m_Width(0), m_Height(0) {}
		virtual ~WindowBackend() {}

        virtual void Create(const int width, const int height, const CTL::String& title) = 0;
		virtual bool WindowShouldClose() const = 0;
		virtual void PollEvents() = 0;

		//virtual void ChangeTitle(const char* title) = 0;

		virtual void* Win32TryGetHWnd() { return nullptr; }
		
		virtual void* GLFWTryGetWnd() { return nullptr; }

		// ImGui
		virtual void ImGuiInitialize() = 0;
		virtual void ImGuiTerminate() = 0;

		virtual void ImGuiNewFrame() = 0;
    private:
		bool m_WindowShouldClose;
		int m_Width, m_Height;
    };
    
	class Window
	{
	public:
		Window();
		~Window();

		void Create(const int width, const int height, const CTL::String& title);
		bool WindowShouldClose() const;
		void PollEvents();

		void* Win32TryGetHWnd();

		void* GLFWTryGetWnd();

		WindowBackend* GetBackend() { return m_Backend; }
    private:
        WindowBackend* m_Backend;
	};
}