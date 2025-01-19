#include "SceneHierarchy.h"
#include "Main/SharedMain.h"

#include "EntitySystem/Scene.h"
#include "EntitySystem/Components/EssentialComponents.h"
#include "Logger/Logger.h"

#include "imgui.h"

using namespace Citrom;

static entt::entity g_SelectedEntity = entt::null;

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

static void DrawHierarchy(Scene* scene)
{
    // probably use .each later on
    auto view = scene->GetAllEntitiesWith<UUIDComponent>();
    for (auto entity : view)
    {
        const UUIDComponent uuidComponent = view.get<UUIDComponent>(entity);
        //ImGui::Text("%zu", uuidComponent.id);
        /*ImGuiTreeNodeFlags flags = ((/*entt::null* / entity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, std::to_string(uuidComponent.id).c_str());
        if (ImGui::IsItemClicked())
        {
            CT_WARN("Clicked");
        }

        if (opened)
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
            bool opened = ImGui::TreeNodeEx((void*)9817239, flags, std::to_string(uuidComponent.id).c_str());
            if (opened)
                ImGui::TreePop();
            ImGui::TreePop();
        }*/

        bool isSelected = (g_SelectedEntity == entity);

        //ImGui::GetWindowDrawList()->AddRectFilled(
        //    ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y),            // Top-left corner
        //    ImVec2(ImGui::GetCursorPos().x + ImVec2(20.0f, 20.0f).x, ImGui::GetCursorPos().y + ImVec2(20.0f, 20.0f).y), // Bottom-right corner
        //    IM_COL32(255, 0, 0, 255)                // Red color (RGBA)
        //);

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.0f);
        if (ImGui::Selectable(std::to_string(uuidComponent.id).c_str(), isSelected /*true /*weird not seeing which entity is selected*/))
        {
            CT_WARN("Entity selected: {}", (uint64)uuidComponent.id); // Log the selection
            g_SelectedEntity = entity;
        }
    }
}

void SceneHierarchyWindow::ImGuiDraw(bool* showWindow)
{
    if (showWindow && !(*showWindow))
        return;

    ImGui::Begin("Scene Hierarchy", showWindow);
    
    DrawRightClickPopupContext();

    DrawHierarchy((Scene*)GetCurrentScene());

    ImGui::End();
}

void* SceneHierarchyWindow::GetSelectedEntity()
{
    return g_SelectedEntity == entt::null ? nullptr : (void*)g_SelectedEntity;
}