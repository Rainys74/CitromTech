#pragma once

#include "CTL/String.h"

/*
* Responsible for the creation of a Surface for
* rendering and handling user interactions.
*/
namespace Citrom::Platform
{
#define PLATFORM_DEFAULT_WIDTH (800)
#define PLATFORM_DEFAULT_HEIGHT (600)

	// TODO: probably move this into a screen file or something
	struct Resolution
	{
		uint32 width, height;
		uint32 refreshRate;

		std::string ToString()
		{
			return std::string();
		}
	};

	enum class DisplayMode // ScreenMode
	{
		Windowed,
		Borderless, // BorderlessWindowed
		Fullscreen
		//ExclusiveFullscreen
	};

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
		virtual void SetDisplayMode(DisplayMode displayMode, const uint32 refreshRate = 0) = 0;
		virtual void SetResolution(const uint32 width, const uint32 height, const uint32 refreshRate = 0, const int xPos = 0, const int yPos = 0) = 0;
		virtual Resolution GetResolution() = 0;

		virtual int GetWidth() = 0;
		virtual int GetHeight() = 0;

		virtual void* Win32TryGetHWnd() { return nullptr; }
        virtual void* CocoaTryGetNSWnd() { return nullptr; }
        virtual void* CocoaTryGetNSView() { return nullptr; }
		
		virtual void* GLFWTryGetWnd() { return nullptr; }

		// ImGui
		virtual void ImGuiInitialize() = 0;
		virtual void ImGuiTerminate() = 0;

		virtual const char* GetName() = 0;
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
        void* CocoaTryGetNSWnd();

		void* GLFWTryGetWnd();

		WindowBackend* GetBackend() { return m_Backend; }
    private:
        WindowBackend* m_Backend;
	};
}
