#include "InspectorWindow.h"
#include "SceneHierarchy.h"
#include "Main/SharedMain.h"

#include "EntitySystem/Scene.h"
#include "EntitySystem/Entity.h"
#include "EntitySystem/Components/EssentialComponents.h"
#include "EntitySystem/Components/RendererComponents.h"
#include "Logger/Logger.h"

#include "CTL/CStringHandling.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "misc/cpp/imgui_stdlib.cpp" // unity build?
#include "ImGuizmo.h"
#include "Vendor/ImGuiNotify/ImGuiNotify.hpp"

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

        Entity frontEntity = Entity(entity, scene);

        auto& uuidComponent = view.get<UUIDComponent>(entity);
        auto& nameComponent = frontEntity.GetComponent<NameComponent>();
        auto& activeComponent = frontEntity.GetComponent<ActiveComponent>();
        auto& transformComponent = frontEntity.GetComponent<TransformComponent>();

        //{
        //    //char* entityName = const_cast<char*>(nameComponent.name.c_str());
        //    char* entityName = new char[nameComponent.name.size()+2];
        //    CTL::CString::Copy(entityName, nameComponent.name.c_str());
        //    ImGui::InputText("##ComponentNameInput", entityName, nameComponent.name.size()+2);
        //    nameComponent.name = std::string(entityName);
        //    delete[] entityName;
        //}
        ImGui::Checkbox("##EntityEnabledToggleTick", &activeComponent.active); // this doesn't work because Entities get constructed every frame.. TODO: move active status into a component

        ImGui::SameLine();
        ImGui::InputText("##ComponentNameInput", &nameComponent.name);

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);

        ImGui::BeginDisabled();
        ImGui::Text("%016llx", (uint64)uuidComponent.id);
        //char formattedUUID[32 + 8 + 1]; // 32 hex chars, 8 dashes, 1 null terminator
        //std::snprintf(formattedUUID, sizeof(formattedUUID), "{%08llx-%08llx-%08llx-%08llx}", (uuidComponent.id >> 48) & 0xFFFF, (uuidComponent.id >> 32) & 0xFFFF, (uuidComponent.id >> 16) & 0xFFFF, uuidComponent.id & 0xFFFF);
        ImGui::EndDisabled();

        ImGui::PopItemWidth();

        //ImGui::Button("TESTNAME");
        //if (ImGui::BeginPopupContextItem())
        //{
        //    ImGui::Text("Edit name:");
        //    ImGui::InputText("##edit", (char*)"NAME", 5);
        //    if (ImGui::Button("Close"))
        //        ImGui::CloseCurrentPopup();
        //    ImGui::EndPopup();
        //}

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
            if (ImGui::MenuItem("Test"))
                ImGui::InsertNotification({ ImGuiToastType::Success, 3000, "That is a success! %s", "(Format here)" });

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