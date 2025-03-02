#include "InspectorWindow.h"
#include "SceneHierarchy.h"
#include "Main/SharedMain.h"

#include "EntitySystem/Scene.h"
#include "EntitySystem/Entity.h"
#include "EntitySystem/Components/EssentialComponents.h"
#include "EntitySystem/Components/RendererComponents.h"
#include "EntitySystem/Components/ScriptComponents.h"
#include "ImGui/ImGuiToolkit.h"
#include "Logger/Logger.h"

#include "SceneHierarchyEvents.h"
#include "NativeFileDialog/NativeFileDialog.h"
#include "Scripting/Native/NativeScript.h"

#include "CTL/CStringHandling.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "misc/cpp/imgui_stdlib.cpp" // unity build?
#include "Vendor/ImGuiNotify/ImGuiNotify.hpp"

#include <filesystem>

using namespace Citrom;

static EventListener<SceneHierarchyEvents> g_HierarchyEventListener;
static entt::entity g_SelectedEntity = entt::null;

static void TryInitializeHierarchyEventListener()
{
    static bool initialized = false;

    if (!initialized)
    {
        g_HierarchyEventListener.OnEvent = [](const Event<SceneHierarchyEvents>& event)
        {
            if (event.GetEventType() == SceneHierarchyEvents::EntitySelect)
            {
                const EntitySelectEvent& transformedEvent = (const EntitySelectEvent&)event;
                g_SelectedEntity = (entt::entity)transformedEvent.entityHandleID;
            }
        };
        EventBus::GetDispatcher<SceneHierarchyEvents>()->AddListener(&g_HierarchyEventListener);

        initialized = true;
    }
}

template<typename T>
T& AddComponentToEntity(Entity& entity, bool closeImGuiPopup = true)
{
    if (entity.HasComponent<T>())
    {
        ImGui::InsertNotification({ ImGuiToastType::Error, 3000, "Citrom Tech does not support multiple duplicate components per entity!"});
        if (closeImGuiPopup)
            ImGui::CloseCurrentPopup();
        return entity.GetComponent<T>();
    }
    T& component = entity.AddComponent<T>();

    if (closeImGuiPopup)
        ImGui::CloseCurrentPopup();

    return component;
}

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
        ImGui::Spacing();
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) // TODO: maybe right-clicking header opens context that allows removing the component?
        {
            ImToolkit::DrawVector3Control("Position", &transformComponent.transform.position[0], 0.25f);
            
            //Math::Vector3 eulerAngles = transformComponent.transform.rotation.ToEulerAngles();
            //if (ImGui::DragFloat3("Rotation", &eulerAngles[0]))
            //    transformComponent.transform.rotation = Math::Quaternion::Euler(eulerAngles);

            Math::Vector3 degreeRot = transformComponent.transform.eulerAnglesHint.RadToDegrees();
            if (ImToolkit::DrawVector3Control("Rotation", &degreeRot[0], 0.1f))
            {
                transformComponent.transform.eulerAnglesHint = degreeRot.DegToRadians();
                transformComponent.transform.rotation = Math::Quaternion::Euler(transformComponent.transform.eulerAnglesHint);
            }

            ImToolkit::DrawVector3Control("Scale", &transformComponent.transform.scale[0]);

            // Advanced
            ImGui::Separator();
            ImToolkit::DrawVector4Control("Quaternion Rotation", &transformComponent.transform.rotation[0]);
            ImGui::Separator();

            // Matrix
            Math::Matrix4x4& mat4 = transformComponent.transform.GetTransformMatrix();
            ImGui::DragFloat4("Row 1", mat4.Data()[0].Data());
            ImGui::DragFloat4("Row 2", mat4.Data()[1].Data());
            ImGui::DragFloat4("Row 3", mat4.Data()[2].Data());
            ImGui::DragFloat4("Row 4", mat4.Data()[3].Data());
        }
        ImGui::Spacing();
        if (frontEntity.HasComponent<CameraComponent>() && ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //ProjectionType m_ProjectionType = ProjectionType::Orthographic;
            //
            //float32 m_PerspectiveFOV = Math::DegreesToRadians(45.0f);
            //float32 m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;
            //
            //float32 m_OrthographicSize = 10.0f;
            //float32 m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
            //
            //float32 m_AspectRatio = 0.0f;
            //
            //Math::Matrix4x4 m_Projection = Math::Matrix4x4::Identity();

            auto& cameraComponent = frontEntity.GetComponent<CameraComponent>();

            static uint8fast projectionType = (uint8fast)cameraComponent.camera.GetProjectionType();

            // Perspective
            ImGui::Separator();
            float32 perspectiveYFOV = Math::RadiansToDegrees(cameraComponent.camera.GetPerspectiveVerticalFOV());
            if (ImToolkit::DrawFloatControl("Vertical FOV", &perspectiveYFOV, 0.1f)) // O for now is the best looking right behind, deg looks weird but ok, Deg looks weird too? Deg. looks weirder, "%.1f deg" looking the best
                cameraComponent.camera.SetPerspectiveVerticalFOV(Math::DegreesToRadians(perspectiveYFOV));

            float32 perspectiveNear = cameraComponent.camera.GetPerspectiveNearClip();
            if (ImToolkit::DrawFloatControl("Near Clip", &perspectiveNear, 0.1f))
                cameraComponent.camera.SetPerspectiveNearClip(perspectiveNear);

            float32 perspectiveFar = cameraComponent.camera.GetPerspectiveFarClip();
            if (ImToolkit::DrawFloatControl("Far Clip", &perspectiveFar, 0.1f))
                cameraComponent.camera.SetPerspectiveFarClip(perspectiveFar);

            // Orthographic
            ImGui::Separator();
            float32 orthoSize = cameraComponent.camera.GetOrthographicSize();
            float32 orthoNear = cameraComponent.camera.GetOrthographicNearClip();
            float32 orthoFar = cameraComponent.camera.GetOrthographicFarClip();

            ImGui::Separator();
            ImGui::ColorEdit4("Clear Color", &cameraComponent.camera.clearColor[0]);

            ImToolkit::DrawVector2Control("Viewport Size", &cameraComponent.camera.viewportSize[0], 0.01f);
            if (cameraComponent.camera.viewportSize.x < 0.0f) 
                cameraComponent.camera.viewportSize.x = 0.0f;
            if (cameraComponent.camera.viewportSize.y < 0.0f)
                cameraComponent.camera.viewportSize.y = 0.0f;

            ImToolkit::DrawVector2Control("Viewport Offset", &cameraComponent.camera.viewportOffset[0], 0.01f);

            // TODO: temporary test!
            Math::Transform* camTransform = (Math::Transform*)GetCameraTransform();

            ImGui::DragFloat3("CCPosition2", &camTransform->position[0]);
            ImGui::DragFloat3("CCEulerAnglesHint2", &camTransform->eulerAnglesHint[0] );
            ImGui::DragFloat3("CCScale2", &camTransform->scale[0]);
            ImGui::Separator();
            ImGui::DragFloat4("CCRotation2", &camTransform->rotation[0]);

            //cameraComponent.camera.SetOrthographicSize(orthoSize);
            //cameraComponent.camera.SetOrthographicNearClip(orthoNear);
            //cameraComponent.camera.SetOrthographicFarClip(orthoFar);
        }
        ImGui::Spacing();
        if (frontEntity.HasComponent<ModelComponent>() && ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto& modelComponent = frontEntity.GetComponent<ModelComponent>();

            static std::string materialPath;
            if (ImToolkit::PathPicker("Material", &materialPath))
            {
                // strip material Path if enter is pressed
                //CT_CORE_WARN("MATERIAL PATH: {}", materialPath);
                //materialPath = "";

                std::filesystem::path path(materialPath);
                materialPath = path.filename().stem().string(); // = filename_with_ext.substr(0, filename_with_ext.find('_'));
            }
        }
        ImGui::Spacing();
        if (frontEntity.HasComponent<NativeScriptComponent>() && ImGui::CollapsingHeader("Native Script", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto& nativeScriptComponent = frontEntity.GetComponent<NativeScriptComponent>();
            
            std::string& behaviourName = nativeScriptComponent.behaviorName;
            bool hasBehaviour = Scripting::NativeScriptDB::HasBehavior(behaviourName);

            if (ImToolkit::DrawStringSetSelector("Script Behavior Name", &behaviourName, CTL::HashMapToHashSet(Scripting::NativeScriptDB::GetBehaviorMap()), !hasBehaviour))
            {
                hasBehaviour = Scripting::NativeScriptDB::HasBehavior(behaviourName);

                if (hasBehaviour)
                    nativeScriptComponent.SetBehaviorWithString(behaviourName);
            }
        }
        ImGui::Separator();
        ImGui::Spacing();

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
            if (ImGui::MenuItem("NFD TEST!"))
            {
                NativeFileDialog nfd;
                NativeFileDialogFilter filters[2] = { { "Source code", "c,cpp,cc" }, { "Headers", "h,hpp" } };
                CT_CORE_WARN("NFD RESULT!: {}", nfd.OpenFile(filters, CT_ARRAY_LENGTH(filters)));
            }

            if (ImGui::TreeNode("Mesh"))
            {
                if (ImGui::Button("Model## Renderer"))
                    AddComponentToEntity<ModelComponent>(frontEntity);

                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Rendering"))
            {
                if (ImGui::Button("Camera"))
                    AddComponentToEntity<CameraComponent>(frontEntity);

                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Scripts")) // vs Scripting
            {
                if (ImGui::Button("Native Script")) // Do we need tree'd groups?
                    AddComponentToEntity<NativeScriptComponent>(frontEntity).SetBehavior<NullScriptBehavior>();

                ImGui::TreePop();
            }

            ImGui::EndPopup();
        }
    }
}

void InspectorWindow::ImGuiDraw(bool* showWindow)
{
    if (showWindow && !(*showWindow))
        return;

    ImGui::Begin("Inspector", showWindow);

    TryInitializeHierarchyEventListener();

    DrawComponentsUUID(g_SelectedEntity, (Scene*)GetCurrentScene());
    //scene->ForEachEntityCallback([&](auto entity)
    //{
    //
    //});

    ImGui::End();
}