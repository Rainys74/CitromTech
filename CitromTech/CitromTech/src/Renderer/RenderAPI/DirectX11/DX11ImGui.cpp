#include "DX11Device.h"

#ifdef CT_PLATFORM_WINDOWS
#include "imgui.h"

#include "backends/imgui_impl_win32.h"

#include "backends/imgui_impl_dx11.h"
#include "CitromAssert.h"

namespace Citrom::RenderAPI
{
	void DX11Device::ImGuiInitGraphicsAPI()
	{
		ImGui_ImplDX11_Init(m_Device.Get(), m_DeviceContext.Get());
	}
	void DX11Device::ImGuiRenderDrawData()
	{
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		CT_ASSERT(false, "test");
	}
}
#endif