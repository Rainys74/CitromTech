#include "InspectorWindow.h"
#include "SceneHierarchy.h"
#include "Main/SharedMain.h"

#include "EntitySystem/Scene.h"
#include "EntitySystem/Entity.h"
#include "EntitySystem/Components/EssentialComponents.h"
#include "EntitySystem/Components/RendererComponents.h"
#include "Logger/Logger.h"

#include "SceneHierarchyEvents.h"
#include "NativeFileDialog/NativeFileDialog.h"

#include "CTL/CStringHandling.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "misc/cpp/imgui_stdlib.cpp" // unity build?
#include "Vendor/ImGuiNotify/ImGuiNotify.hpp"

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

namespace ImToolkit //ImPresets
{
    static bool _DrawControlElementDragButton(const char* label, const char* invisibleDragLabel, float* value, float speed, const ImVec2& buttonSize, const ImVec4& color, const ImVec4& highlightedColor)
    {
        bool modified = false;

        // Render Rect
        auto cursorPos = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddRectFilled(cursorPos,
            ImVec2(cursorPos.x + buttonSize.x, cursorPos.y + buttonSize.y), ImGui::GetColorU32(color), ImGui::GetStyle().FrameRounding);

        //ImGui::RenderTextClipped(textPos, ImVec2(cursorPos.x + buttonSize.x, cursorPos.y + buttonSize.y), "X", NULL, &textSize, ImVec2(0.5f, 0.5f));

        //ImVec2 textSize = ImGui::CalcTextSize("X");
        //
        //// Compute position to center the text
        //ImVec2 textPos = ImVec2(
        //    cursorPos.x + (buttonSize.x - textSize.x) * 0.5f,  // Center X
        //    cursorPos.y + (buttonSize.y - textSize.y) * 0.5f   // Center Y
        //);
        //
        //// Manually set cursor position for text
        //ImGui::SetCursorScreenPos(textPos);

        // Render Text
        ImVec2 textSize = ImGui::CalcTextSize(label);
        ImVec2 textPos = ImVec2(
            cursorPos.x + (buttonSize.x - textSize.x) * 0.5f,  // Center X
            cursorPos.y + (buttonSize.y - textSize.y) * 0.5f   // Center Y
        );

        auto cursorScreenPosBackup = ImGui::GetCursorScreenPos();
        ImGui::SetCursorScreenPos(textPos);

        ImGui::Text(label);
        ImGui::SetCursorScreenPos(cursorScreenPosBackup);

        ImGui::PushStyleColor(ImGuiCol_Header, color);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, highlightedColor);
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, color);
        ImGui::Selectable(invisibleDragLabel, false, 0, buttonSize);
        if (ImGui::IsItemActive())
        {
            float dragDelta = ImGui::GetIO().MouseDelta.x * speed; // Drag affects X value
            value[0] += dragDelta;
            modified = true;
        }
        ImGui::PopStyleColor(3);

        //ImGui::GetWindowDrawList()->AddText(textPos, ImGui::GetColorU32(ImGuiCol_Text), label);

        return modified;
    }

    static bool DrawVector3Control(const char* label, float values[3], float speed = 0.25f, float resetValue = 0.0f)
    {
        /* // Fresh Start
        bool modified = false;

        ImGui::PushID(label);

        ImGui::Columns(2);

        // Column 1
        ImGui::SetColumnWidth(0, 100.0f);
        ImGui::Text(label);

        ImGui::NextColumn();
        // --------

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0)); // TODO: buttons and sliders should be close, each values should be spaced out.

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = ImVec2{lineHeight + 3.0f, lineHeight};
        
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
        if (ImGui::Button("X", buttonSize));
        ImGui::PopStyleColor(2);

        ImGui::SameLine();
        modified = ImGui::DragFloat("##X", &values[0], speed);
        ImGui::PopItemWidth();

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
        if (ImGui::Button("Y", buttonSize));
        ImGui::PopStyleColor(2);

        ImGui::SameLine();
        modified = ImGui::DragFloat("##Y", &values[1], speed);
        ImGui::PopItemWidth();

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
        if (ImGui::Button("Z", buttonSize));
        ImGui::PopStyleColor(2);

        ImGui::SameLine();
        modified = ImGui::DragFloat("##Z", &values[2], speed);
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();

        return modified;
        */
        bool modified = false;

        ImGui::PushID(label);

        ImGui::Columns(2);

        // Column 1
        ImGui::SetColumnWidth(0, 100.0f);
        ImGui::Text(label);

        ImGui::NextColumn();
        // --------

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0)); // TODO: buttons and sliders should be close, each values should be spaced out.

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = ImVec2{ lineHeight + 3.0f, lineHeight };

        // X
        modified |= _DrawControlElementDragButton("X", "##X_SelectableDrag", &values[0], speed, buttonSize, ImVec4(0.8f, 0.1f, 0.15f, 1.0f), ImVec4(0.9f, 0.2f, 0.2f, 1.0f));

        ImGui::SameLine();
        modified |= ImGui::DragFloat("##X", &values[0], speed);
        ImGui::PopItemWidth();

        // Y
        ImGui::SameLine();
        modified |= _DrawControlElementDragButton("Y", "##Y_SelectableDrag", &values[1], speed, buttonSize, ImVec4(0.2f, 0.7f, 0.2f, 1.0f), ImVec4(0.3f, 0.8f, 0.3f, 1.0f));

        ImGui::SameLine();
        modified |= ImGui::DragFloat("##Y", &values[1], speed);
        ImGui::PopItemWidth();

        // Z
        ImGui::SameLine();
        modified |= _DrawControlElementDragButton("Z", "##Z_SelectableDrag", &values[2], speed, buttonSize, ImVec4(0.1f, 0.25f, 0.8f, 1.0f), ImVec4(0.2f, 0.35f, 0.9f, 1.0f));

        ImGui::SameLine();
        modified |= ImGui::DragFloat("##Z", &values[2], speed);
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();

        return modified;
    }
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
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
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
            ImGui::DragFloat4("Quaternion Rotation", &transformComponent.transform.rotation[0]); // Quaternion
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
            static float32 perspectiveYFOV = cameraComponent.camera.GetPerspectiveVerticalFOV();
            static float32 perspectiveNear = cameraComponent.camera.GetPerspectiveNearClip();
            static float32 perspectiveFar = cameraComponent.camera.GetPerspectiveFarClip();

            // Orthographic
            static float32 orthoSize = cameraComponent.camera.GetOrthographicSize();
            static float32 orthoNear = cameraComponent.camera.GetOrthographicNearClip();
            static float32 orthoFar = cameraComponent.camera.GetOrthographicFarClip();

            // TODO: temporary test!
            Math::Transform* camTransform = (Math::Transform*)GetCameraTransform();

            ImGui::DragFloat3("CCPosition2", &camTransform->position[0]);
            ImGui::DragFloat3("CCEulerAnglesHint2", &camTransform->eulerAnglesHint[0] );
            ImGui::DragFloat3("CCScale2", &camTransform->scale[0]);
            ImGui::Separator();
            ImGui::DragFloat4("CCRotation2", &camTransform->rotation[0]);

            cameraComponent.camera.SetPerspectiveVerticalFOV(perspectiveYFOV);
            cameraComponent.camera.SetPerspectiveNearClip(perspectiveNear);
            cameraComponent.camera.SetPerspectiveFarClip(perspectiveFar);

            cameraComponent.camera.SetOrthographicSize(orthoSize);
            cameraComponent.camera.SetOrthographicNearClip(orthoNear);
            cameraComponent.camera.SetOrthographicFarClip(orthoFar);
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