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
		CT_CORE_VERIFY(ImGui_ImplDX11_Init(m_Device, m_DeviceContext), "Failed to initialize ImGui DirectX11 implementation.");
	}
	void DX11Device::ImGuiNewFrame()
	{
		ImGui_ImplDX11_NewFrame();
	}
	void DX11Device::ImGuiRenderDrawData(void* imDrawData)
	{
		ImGui_ImplDX11_RenderDrawData((ImDrawData*)imDrawData);
	}
}
#endif