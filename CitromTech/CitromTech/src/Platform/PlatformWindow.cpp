#include "Platform.h"

#include "Core.h"
#include "CitromAssert.h"

#if defined(CT_PLATFORM_WINDOWS)
#elif defined(CT_PLATFORM_LINUX)
#elif defined(CT_PLATFORM_MACOS)
#include "MacOS/PlatformWindowMacOS.h"
#else
#error "The current platform is unsupported!"
#endif

namespace Citrom::Platform
{
    Window* Window::GenerateWindow()
	{
		#if defined(CT_PLATFORM_WINDOWS)
        #elif defined(CT_PLATFORM_LINUX)
        #elif defined(CT_PLATFORM_MACOS)
        return new MacOSWindow();
        #endif

		CT_CORE_ASSERT(false, "Failed to create a window instance.");
		return nullptr;
	}
}