#include "DX11Device.h"

#ifdef CT_PLATFORM_WINDOWS
#include "imgui.h"

#include "backends/imgui_impl_win32.h"

#include "backends/imgui_impl_dx11.h"

namespace Citrom::RenderAPI
{
	void DX11Device::ImGuiInitGraphicsAPI()
	{
		//ImGui_ImplDX11_Init(m_Device, m_DeviceContext);
	}
	void DX11Device::ImGuiRenderDrawData()
	{
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}
#endif