#pragma once

#include "Core.h"

#ifdef CT_PLATFORM_MACOS
namespace Citrom::Platform::Metal
{
    void ImGuiTerminateGLFW();
}
#endif
