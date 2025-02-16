#include "GLDevice.h"

#include "imgui.h"

#include "backends/imgui_impl_opengl3.h"
#include "CitromAssert.h"

namespace Citrom::RenderAPI
{
	void GLDevice::ImGuiInitGraphicsAPI()
	{
        CT_CORE_VERIFY(ImGui_ImplOpenGL3_Init("#version 410"), "Failed to Initialize ImGui OpenGL 4.1 implementation.");
	}
	void GLDevice::ImGuiNewFrame(CommandBuffer* cmd)
	{
		ImGui_ImplOpenGL3_NewFrame();
	}
	void GLDevice::ImGuiRenderDrawData(void* imDrawData, CommandBuffer* cmd)
	{
		ImGui_ImplOpenGL3_RenderDrawData((ImDrawData*)imDrawData);
	}
}
