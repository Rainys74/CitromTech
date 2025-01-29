#include "SceneHierarchy.h"
#include "Main/SharedMain.h"

#include "EntitySystem/Scene.h"
#include "EntitySystem/Entity.h"
#include "EntitySystem/Components/EssentialComponents.h"
#include "Logger/Logger.h"

#include "SceneHierarchyEvents.h"

#include "imgui.h"

using namespace Citrom;

static entt::entity g_SelectedEntt = entt::null;
//static Entity g_SelectedEntity = Entity(entt::null, nullptr);

static void FireEntitySelectionEvent()
{
    EntitySelectEvent entitySelectionEvent;
    entitySelectionEvent.entityHandleID = (EventEntityHandleType)g_SelectedEntt;

    EventBus::GetDispatcher<SceneHierarchyEvents>()->Dispatch(entitySelectionEvent);
}
static void SetSelectedEntity(entt::entity selectedEntt)
{
    g_SelectedEntt = selectedEntt;
    FireEntitySelectionEvent();
}

static Entity CreateEmptyEntity(Scene* scene)
{
    return scene->CreateEntity();
}

static void DrawRightClickPopupContext(Scene* currentScene)
{
    if (ImGui::BeginPopupContextWindow("HierarchyRightClickMenu", ImGuiPopupFlags_MouseButtonRight))
    {
        if (g_SelectedEntt == entt::null)
        {
            ImGui::BeginDisabled(); // Disable if no object is selected
            if (ImGui::MenuItem("Rename"));
            if (ImGui::MenuItem("Delete"));
            ImGui::EndDisabled(); // End disabling if no object is selected
        }
        else
        {
            if (ImGui::MenuItem("Rename"))
            {
                Entity(g_SelectedEntt, currentScene).GetComponent<NameComponent>().name = "Renamed";
            }
            if (ImGui::MenuItem("Delete"))
            {
                currentScene->DestroyEntity(Entity(g_SelectedEntt, currentScene));
                SetSelectedEntity(entt::null);
            }
        }

        ImGui::Separator();
        if (ImGui::MenuItem("Create Empty Object"))
        {
            Entity emptyEntity = CreateEmptyEntity(currentScene);
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
        const NameComponent& nameComponent = Entity(entity, scene).GetComponent<NameComponent>();
        const ActiveComponent& activeComponent = Entity(entity, scene).GetComponent<ActiveComponent>();
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

        bool isSelected = (g_SelectedEntt == entity);

        //ImGui::GetWindowDrawList()->AddRectFilled(
        //    ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y),            // Top-left corner
        //    ImVec2(ImGui::GetCursorPos().x + ImVec2(20.0f, 20.0f).x, ImGui::GetCursorPos().y + ImVec2(20.0f, 20.0f).y), // Bottom-right corner
        //    IM_COL32(255, 0, 0, 255)                // Red color (RGBA)
        //);

        if (!activeComponent.active)
        {
            //ImGuiContext& g = *ImGui::GetCurrentContext();
            //
            //g.DisabledAlphaBackup = g.Style.Alpha;
            //g.Style.Alpha *= g.Style.DisabledAlpha; // PushStyleVar(ImGuiStyleVar_Alpha, g.Style.Alpha * g.Style.DisabledAlpha);

            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        }

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.0f);
        if (ImGui::Selectable(std::string(nameComponent.name).append("##").append(std::to_string(uuidComponent.id)).c_str(), isSelected /*true /*weird not seeing which entity is selected*/))
        {
            CT_WARN("Entity selected: {}", (uint64)uuidComponent.id); // Log the selection
            SetSelectedEntity(entity);

            //g_SelectedEntity = Entity(g_SelectedEntt, scene);
        }

        if (!activeComponent.active) 
            ImGui::PopStyleColor();
    }

    if (ImGui::IsMouseClicked(0) && !ImGui::IsAnyItemHovered() && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
        SetSelectedEntity(entt::null);
}

void SceneHierarchyWindow::ImGuiDraw(bool* showWindow)
{
    if (showWindow && !(*showWindow))
        return;

    ImGui::Begin("Scene Hierarchy", showWindow);
    
    DrawRightClickPopupContext((Scene*)GetCurrentScene());

    DrawHierarchy((Scene*)GetCurrentScene());

    ImGui::End();
}