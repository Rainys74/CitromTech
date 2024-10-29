#ifdef CT_PLATFORM_MACOS
#include "PlatformWindowMacOS.h"

#include "Platform/Shared/PlatformWindowBackendGLFW.h"

namespace Citrom::Platform
{
        MacOSWindow::MacOSWindow()
            : m_Backend(nullptr)
        {
            m_Backend = new WindowBackendGLFW();
        }
		MacOSWindow::~MacOSWindow()
        {
            delete m_Backend;
        }

		void MacOSWindow::Create(const int width, const int height, const CTL::String& title)
        {
            m_Backend->Create(width, height, title);
        }
        bool MacOSWindow::WindowShouldClose() const
        {
            return m_Backend->WindowShouldClose();
        }
		void MacOSWindow::PollEvents()
        {
            m_Backend->PollEvents();
        }
}
#endif