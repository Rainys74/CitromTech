#include "EditorGizmos.h"
#include "Main/SharedMain.h"

#include "EntitySystem/Scene.h"
#include "EntitySystem/Entity.h"
#include "EntitySystem/Components/EssentialComponents.h"
#include "EntitySystem/Components/RendererComponents.h"
#include "Logger/Logger.h"

#include "SceneHierarchyEvents.h"

#include "imgui.h"
#include "ImGuizmo.h"

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

void EditorGizmos::ImGuiDraw(uint16 showGizmos)
{
    if (showGizmos <= 0)
        return;

    TryInitializeHierarchyEventListener();

    //ImGui::Begin("ImGuizmo Gizmos");

    if (g_SelectedEntity != entt::null)
    {
        auto camera = (Camera*)GetCamera();
        auto cameraTransform = (Math::Transform*)GetCameraTransform();

        ImGuizmo::SetOrthographic(camera->GetProjectionType() == Camera::ProjectionType::Orthographic);
        //ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList()); // ImGui::GetBackgroundDrawList()
        //ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
        ImGuizmo::SetRect(0.0f, 0.0f, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);

        const Math::Matrix4x4& cameraProjection = camera->GetProjection();
        Math::Matrix4x4 cameraView = Math::Matrix4x4::Inverse(cameraTransform->GetTransformMatrix());

        CT_ERROR("CAMERA PROJECTION MATRIX: \n{}", camera->GetProjection().ToString());
        CT_ERROR("CAMERA TRANSFORMATION: \n POSITION: {}\n ROTATION: {}\n SCALE: {}\n", cameraTransform->position.ToString(), cameraTransform->rotation.ToString(), cameraTransform->scale.ToString());
        CT_ERROR("CAMERA TRANSFORM MATRIX: \n{}", cameraTransform->GetTransformMatrix().ToString());
        CT_ERROR("CAMERA VIEW MATRIX (TRANSFORM INVERSE): \n{}", Math::Matrix4x4::Inverse(cameraTransform->GetTransformMatrix()).ToString());

        // Entity Transform
        auto& etc = Entity(g_SelectedEntity, (Scene*)GetCurrentScene()).GetComponent<TransformComponent>();
        Math::Matrix4x4 entityTransform = etc.transform.GetTransformMatrix();

        CT_ERROR("ENTITY TRANSFORM MATRIX: \n{}", etc.transform.GetTransformMatrix().ToString());

        ImGuizmo::Manipulate(&cameraView.Data()[0][0], &cameraProjection.Data()[0][0], (ImGuizmo::OPERATION)showGizmos, ImGuizmo::LOCAL /*ImGuizmo::WORLD*/, &entityTransform.Data()[0][0]);

        if (ImGuizmo::IsUsing())
        {
            // Row-Major
            //etc.transform.position.x = entityTransform.Data()[3][0];
            //etc.transform.position.y = entityTransform.Data()[3][1];
            //etc.transform.position.z = entityTransform.Data()[3][2];

            Math::Vector3 translation, eulerAngles, scale;
            Math::Matrix4x4::DecomposeTransform(entityTransform, translation, eulerAngles, scale);

            etc.transform.position = translation;
            //etc.transform.eulerAnglesHint = eulerAngles;
            etc.transform.editorEulerAngles = eulerAngles;
            //etc.transform.rotation = Math::Quaternion::Euler(eulerAngles);
            etc.transform.scale = scale; // TODO: scale keeps increasing, might be due to the square root not being that accurate?
        }
    }

    //ImGui::End();
}