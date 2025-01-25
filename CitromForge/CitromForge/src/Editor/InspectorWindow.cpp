#include "InspectorWindow.h"
#include "SceneHierarchy.h"
#include "Main/SharedMain.h"

#include "EntitySystem/Scene.h"
#include "EntitySystem/Entity.h"
#include "EntitySystem/Components/EssentialComponents.h"
#include "EntitySystem/Components/RendererComponents.h"
#include "Logger/Logger.h"

#include "imgui.h"
#include "ImGuizmo.h"

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
        auto& nameComponent = Entity(entity, scene).GetComponent<NameComponent>();
        auto& transformComponent = Entity(entity, scene).GetComponent<TransformComponent>();

        //ImGui::InputText("##ComponentNameInput", );
        ImGui::Text("Test1");

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);

        ImGui::BeginDisabled();
        //ImGui::Text("%X", uuidComponent.id);
        //ImGui::Text("%08X", uuidComponent.id);
        //ImGui::Text("%016llX", (uint64)uuidComponent.id);
        ImGui::Text("%016llx", (uint64)uuidComponent.id);
        ImGui::EndDisabled();

        ImGui::PopItemWidth();

        if (ImGui::CollapsingHeader("General", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::BeginDisabled();
            ImGui::Text("%zu", uuidComponent.id);
            ImGui::Text("%s", nameComponent.name.c_str());
            ImGui::EndDisabled();
        }
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::DragFloat3("Position", &transformComponent.transform.position[0]);
            ImGui::DragFloat4("Rotation", &transformComponent.transform.rotation[0]);
            ImGui::DragFloat3("Scale", &transformComponent.transform.scale[0]);
        }
        ImGui::Separator();

        // Add Component
        constexpr float buttonWidth = 200.0f;

        const float offsetX = (ImGui::GetContentRegionAvail().x - buttonWidth) * 0.5f;
        if (offsetX > 0.0f)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);

        if (ImGui::Button("Add Component", ImVec2(buttonWidth, 0.0f)))
        {
            ImGui::OpenPopup("ComponentAdder");
        }
        
        if (ImGui::BeginPopup("ComponentAdder"))
        {
            if (ImGui::MenuItem("Test"));

            ImGui::EndPopup();
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

    // TODO: get this shit out of here
    if (selectedEntity != entt::null)
    {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

        // TODO: this is NOT an editor camera!
        auto cameraEntity = ((Scene*)GetCurrentScene())->GetMainCameraEntity();
    }

    ImGui::End();
}