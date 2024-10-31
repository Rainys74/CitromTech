#pragma once

#ifdef CT_PLATFORM_WINDOWS
#include "Platform/PlatformWindow.h"

namespace Citrom::Platform
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow();
		virtual ~WindowsWindow();

		virtual void Create(const int width, const int height, const CTL::String& title) override;
		virtual bool WindowShouldClose() const override;
		virtual void PollEvents() override;
	private:
		WindowBackend* m_Backend;
	};
}
#endif