#include "SceneHierarchy.h"

#include "imgui.h"

//using namespace Citrom;

static void DrawRightClickPopupContext()
{
    if (ImGui::BeginPopupContextWindow("HierarchyRightClickMenu", ImGuiPopupFlags_MouseButtonRight))
    {
        ImGui::BeginDisabled(); // Disable if no object is selected
        if (ImGui::MenuItem("Rename"))
        {

        }
        if (ImGui::MenuItem("Delete"))
        {

        }
        ImGui::EndDisabled(); // End disabling if no object is selected

        ImGui::Separator();
        if (ImGui::MenuItem("Create Empty Object"))
        {

        }
        if (ImGui::BeginMenu("Create Light"))
        {
            if (ImGui::MenuItem("Point Light"))
            {
                // Add logic to create a point light entity
            }
            if (ImGui::MenuItem("Directional Light"))
            {
                // Add logic to create a directional light entity
            }

            ImGui::EndMenu();
        }

        ImGui::EndPopup();
    }
}

void SceneHierarchyWindow::ImGuiDraw(bool* showWindow)
{
    if (showWindow && !(*showWindow))
        return;

    ImGui::Begin("Scene Hierarchy", showWindow);
    
    DrawRightClickPopupContext();

    ImGui::End();
}