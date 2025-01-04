#include "PreferenceWindow.h"

#include "ImGui/ImGuiStyles.h"

#include "imgui.h"

using namespace Citrom;

void PreferenceWindow::ImGuiDraw(bool* showWindow)
{
    if (!showWindow || !(*showWindow))
        return;

    ImGui::Begin("Preferences", showWindow);
    /*if (ImGui::BeginTabBar("PreferenceTabs"))
    {
        if (ImGui::BeginTabItem("Editor"))
        {
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }*/

    static int currentEditorStyle = 0;

    // Simple:
    //ImGui::Combo("Editor Style", &currentEditorStyle, [](void* data, int n) {((ImGuiStyleObj*)data)[currentEditorStyle].callback(); return ((ImGuiStyleObj*)data)[n].name.c_str(); }, ImGuiStyles::GetStyles().Data(), ImGuiStyles::GetStyles().Count());
    //ImGuiStyles::GetStyles().Data()[currentEditorStyle].callback();
    // or Complex (better):
    const char* combo_preview_value = ImGuiStyles::GetStyles().Data()[currentEditorStyle].name.c_str();
    if (ImGui::BeginCombo("Editor Style", combo_preview_value, 0))
    {
        for (int i = 0; i < ImGuiStyles::GetStyles().Count(); i++)
        {
            const ImGuiStyleObj& style = ImGuiStyles::GetStyles().Data()[i];
            const bool is_selected = (currentEditorStyle == i);

            if (ImGui::Selectable(style.name.c_str(), is_selected))
            {
                currentEditorStyle = i; // Update the selected index
                style.callback();      // Call the callback for the newly selected style
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus(); // Set focus on the selected item for keyboard navigation
        }
        ImGui::EndCombo();
    }

    ImGui::End();
}