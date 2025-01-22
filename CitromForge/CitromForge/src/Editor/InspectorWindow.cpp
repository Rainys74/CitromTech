#include "InspectorWindow.h"
#include "SceneHierarchy.h"
#include "Main/SharedMain.h"

#include "EntitySystem/Scene.h"
#include "EntitySystem/Entity.h"
#include "EntitySystem/Components/EssentialComponents.h"
#include "EntitySystem/Components/RendererComponents.h"
#include "Logger/Logger.h"

#include "imgui.h"

using namespace Citrom;

static void DrawComponentsUUID(entt::entity selectedEntity, Scene* scene)
{
    if (selectedEntity == entt::null)
        return;

    auto view = scene->GetAllEntitiesWith<UUIDComponent>();
    for (auto entity : view)
    {
        if (entity != selectedEntity)
            continue;

        auto& uuidComponent = view.get<UUIDComponent>(entity);
        auto& transformComponent = Entity(entity, scene).GetComponent<TransformComponent>();

        if (ImGui::CollapsingHeader("General", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("%zu", uuidComponent.id);
        }
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //static Math::Vector3 position;
            //static Math::Quaternion rotation;
            //static Math::Vector3 scale;
            ImGui::DragFloat3("Position", &transformComponent.position[0]);
            ImGui::DragFloat4("Rotation", &transformComponent.rotation[0]);
            ImGui::DragFloat3("Scale", &transformComponent.scale[0]);
        }
        ImGui::Separator();

        // Add Component
        constexpr float buttonWidth = 200.0f;

        const float offsetX = (ImGui::GetContentRegionAvail().x - buttonWidth) * 0.5f;
        if (offsetX > 0.0f)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);

        if (ImGui::Button("Add Component", ImVec2(buttonWidth, 0.0f)))
        {
            
        }
    }
}

void InspectorWindow::ImGuiDraw(bool* showWindow)
{
    if (showWindow && !(*showWindow))
        return;

    ImGui::Begin("Inspector", showWindow);

    entt::entity selectedEntity = (entt::entity)SceneHierarchyWindow::GetSelectedEntity();

    DrawComponentsUUID(selectedEntity, (Scene*)GetCurrentScene());
    //scene->ForEachEntityCallback([&](auto entity)
    //{
    //
    //});

    ImGui::End();
}