#include "ProjectSettingsWindow.h"
#include "Main/SharedMain.h"

#include "Logger/Logger.h"
#include "Renderer/RenderAPI/GraphicsDevice.h"

#include "imgui.h"

using namespace Citrom;

static const char* VSyncModeToString(RenderAPI::VSyncMode mode)
{
#define TO_STRING_CASE(Enum) case Enum: return #Enum; break
    using namespace RenderAPI;
    switch (mode)
    {
        //TO_STRING_CASE(VSyncMode::Adaptive);

        TO_STRING_CASE(VSyncMode::Off);
        TO_STRING_CASE(VSyncMode::On);
        TO_STRING_CASE(VSyncMode::Half);
        TO_STRING_CASE(VSyncMode::Quarter);
        TO_STRING_CASE(VSyncMode::Eighth);

        default: return "Unknown";
    }
    return "Unknown";
}

template<typename T, const char* (*TToString)(T)>
static bool ComboEnum(const char* label, T& currentValue, const T* values, int valuesCount, const int valuesBegin = 0) 
{
    bool valueChanged = false;
    const char* previewValue = TToString(currentValue);

    if (ImGui::BeginCombo(label, previewValue))
    {
        for (int n = valuesBegin; n < valuesCount; n++)
        {
            bool selected = (currentValue == values[n]);
            if (ImGui::Selectable(TToString(values[n]), selected))
            {
                currentValue = values[n];
                valueChanged = true;
            }
            if (selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    return valueChanged;
}

void ProjectSettingsWindow::ImGuiDraw(bool* showWindow)
{
    if (showWindow && !(*showWindow))
        return;

    ImGui::Begin("Project Settings");
    if (ImGui::BeginTabBar("ProjectSettingTabs"))
    {
        if (ImGui::BeginTabItem("Graphics"))
        {
            //RenderAPI::Device::Get()->SetVSync
            //RenderAPI::Device::Get()->GetVSync
            //const char* combo_preview_value = TO_STRING(RenderAPI::VSyncMode::On);
            //if (ImGui::BeginCombo("V-Sync Interval", combo_preview_value, 0))
            //{
            //    for (int i = -1; i < 4; i++)
            //    {
            //        const RenderAPI::VSyncMode& vSync = RenderAPI::VSyncMode::On;
            //        const bool selected = ((int)RenderAPI::VSyncMode::On == i);
            //
            //        if (ImGui::Selectable(TO_STRING(vSync), selected))
            //        {
            //            //currentEditorStyle = i; // Update the selected index
            //        }
            //
            //        if (selected)
            //            ImGui::SetItemDefaultFocus(); // Set focus on the selected item for keyboard navigation
            //    }
            //    ImGui::EndCombo();
            //}
            RenderAPI::VSyncMode currentVSyncMode = RenderAPI::Device::Get()->GetVSync();
            static constexpr RenderAPI::VSyncMode availableVSyncModes[] = { /*RenderAPI::VSyncMode::Adaptive,*/ RenderAPI::VSyncMode::Off, RenderAPI::VSyncMode::On, RenderAPI::VSyncMode::Half, RenderAPI::VSyncMode::Quarter, RenderAPI::VSyncMode::Eighth};
            if (ComboEnum<RenderAPI::VSyncMode, VSyncModeToString>("V-Sync Interval", currentVSyncMode, availableVSyncModes, CT_ARRAY_LENGTH(availableVSyncModes)))
            {
                CT_CORE_INFO("TESTT!~!! {}", (int)currentVSyncMode);
                RenderAPI::Device::Get()->SetVSync(currentVSyncMode);
            }

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Input System"))
        {
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Input System2")) ImGui::EndTabItem();

        ImGui::EndTabBar();
    }
    ImGui::End();
}