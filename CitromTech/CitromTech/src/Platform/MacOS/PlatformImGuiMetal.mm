#include "PlatformImGuiMetal.h"

#ifdef CT_PLATFORM_MACOS
#include "backends/imgui_impl_metal.h"

#include "backends/imgui_impl_glfw.h"

namespace Citrom::Platform::Metal
{
    void ImGuiTerminateGLFW()
    {
        ImGui_ImplMetal_Shutdown();
    }
}
#endif
