#include "PreferenceWindow.h"

#include "ImGui/ImGuiStyles.h"

#include "imgui.h"

using namespace Citrom;

static void DrawEditorStyle()
{
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
}

static void DrawUIScaling()
{
    static float uiScale = 1.0f;

    //ImGuiStyle& style = ImGui::GetStyle();
    //style.ScaleAllSizes(uiScale); // continuously increases the size. leads to a memory leak.

    //ImGui::GetIO().FontGlobalScale = uiScale; // too blurry
    //ImGui::GetIO().DisplayFramebufferScale = ImVec2(uiScale, uiScale); // does seemingly nothing?

    if (ImGui::InputFloat("UI Scaling", &uiScale))
    {
        if (uiScale > 3.0f)
            uiScale = 3.0f;
        if (uiScale < 0.5f)
            uiScale = 0.5f;

        // TODO: Best chance: when loading the font, change the font size: that scales the entire application perfectly.
        // But for now: cope
        ImGui::GetIO().FontGlobalScale = uiScale;
    }
}

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

    DrawEditorStyle();
    DrawUIScaling();

    ImGui::End();
}