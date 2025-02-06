#include "ProjectSettingsWindow.h"
#include "Main/SharedMain.h"
#include "ImGui/ImGuiToolkit.h"

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
        TO_STRING_CASE(VSyncMode::Adaptive);

        TO_STRING_CASE(VSyncMode::Off);
        TO_STRING_CASE(VSyncMode::On);
        TO_STRING_CASE(VSyncMode::Half);
        TO_STRING_CASE(VSyncMode::Quarter);
        TO_STRING_CASE(VSyncMode::Eighth);

        default: return "Unknown";
    }
    return "Unknown";
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
            static constexpr RenderAPI::VSyncMode availableVSyncModes[] = { RenderAPI::VSyncMode::Adaptive, RenderAPI::VSyncMode::Off, RenderAPI::VSyncMode::On, RenderAPI::VSyncMode::Half, RenderAPI::VSyncMode::Quarter, RenderAPI::VSyncMode::Eighth};
            if (ImToolkit::ComboEnum<RenderAPI::VSyncMode, VSyncModeToString>("V-Sync Interval", currentVSyncMode, availableVSyncModes, CT_ARRAY_LENGTH(availableVSyncModes)))
            {
                CT_CORE_INFO("TESTT!~!! {}", (int)currentVSyncMode);
                RenderAPI::Device::Get()->SetVSync(currentVSyncMode);
            }

            // use for graphics api
            /*
                IMGUI_DEMO_MARKER("Widgets/Drag and Drop/Drag to reorder items (simple)");
                if (ImGui::TreeNode("Drag to reorder items (simple)"))
                {
                    // FIXME: there is temporary (usually single-frame) ID Conflict during reordering as a same item may be submitting twice.
                    // This code was always slightly faulty but in a way which was not easily noticeable.
                    // Until we fix this, enable ImGuiItemFlags_AllowDuplicateId to disable detecting the issue.
                    ImGui::PushItemFlag(ImGuiItemFlags_AllowDuplicateId, true);

                    // Simple reordering
                    HelpMarker(
                        "We don't use the drag and drop api at all here! "
                        "Instead we query when the item is held but not hovered, and order items accordingly.");
                    static const char* item_names[] = { "Item One", "Item Two", "Item Three", "Item Four", "Item Five" };
                    for (int n = 0; n < IM_ARRAYSIZE(item_names); n++)
                    {
                        const char* item = item_names[n];
                        ImGui::Selectable(item);

                        if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
                        {
                            int n_next = n + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
                            if (n_next >= 0 && n_next < IM_ARRAYSIZE(item_names))
                            {
                                item_names[n] = item_names[n_next];
                                item_names[n_next] = item;
                                ImGui::ResetMouseDragDelta();
                            }
                        }
                    }

                    ImGui::PopItemFlag();
                    ImGui::TreePop();
                }
            */

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