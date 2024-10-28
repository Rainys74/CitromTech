#pragma once

#include "Platform/PlatformWindow.h"

namespace Citrom::Platform
{
    class MacOSWindow : public Window
	{
	public:
		MacOSWindow();
		virtual ~MacOSWindow();

		virtual void Create(const int width, const int height, const CTL::String& title) override;
		virtual bool WindowShouldClose() const override;
		virtual void PollEvents() override;
	private:
		WindowBackend* m_Backend;
	};
}