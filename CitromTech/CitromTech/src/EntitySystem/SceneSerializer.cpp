#include "SceneSerializer.h"
#include "EntitySystem/Entity.h"

#include "Components/EssentialComponents.h"
#include "Components/RendererComponents.h"

#include "JSON/Writer.h"
#include "JSON/Reader.h"

#include <string>
#include <fstream>

namespace Citrom
{
	// Serialization
	void Scene::SerializeJson(rapidjson::Document& doc, rapidjson::Document::AllocatorType& allocator)
	{
		doc.SetObject();

		JSON_WRITER_SET_STRING("SceneName", std::string("Untitled"));

		//doc.SetArray();

		rapidjson::Value entityArray(rapidjson::kArrayType);
		for (const auto [entityID, uuidComponent] : m_SceneRegistry.storage<UUIDComponent>().each())
		{
			Entity entity = Entity(entityID, this);

			const auto& nameComponent = entity.GetComponent<NameComponent>();
			const auto& activeComponent = entity.GetComponent<ActiveComponent>();
			const auto& hierarchyComponent = entity.GetComponent<HierarchyComponent>();
			const auto& transformComponent = entity.GetComponent<TransformComponent>();

			rapidjson::Value entityObject(rapidjson::kObjectType);
			entityObject.AddMember("EntityName", rapidjson::Value(nameComponent.name.c_str(), allocator), allocator);
			//entityObject.AddMember("UUID", (uint64)uuidComponent.id, allocator);

			rapidjson::Value componentArray(rapidjson::kObjectType);

			if (entity.HasComponent<UUIDComponent>())
			{
				rapidjson::Value guidComponentArray(rapidjson::kObjectType);
				guidComponentArray.AddMember("GUID", (uint64)uuidComponent.id, allocator);
				componentArray.AddMember("UUIDComponent", guidComponentArray, allocator);
			}
			if (entity.HasComponent<NameComponent>())
			{
				rapidjson::Value tagComponentArray(rapidjson::kObjectType);
				tagComponentArray.AddMember("Name", rapidjson::Value(nameComponent.name.c_str(), allocator), allocator);
				componentArray.AddMember("NameComponent", tagComponentArray, allocator);
			}
			if (entity.HasComponent<ActiveComponent>())
			{
				rapidjson::Value enabledComponentArray(rapidjson::kObjectType);
				enabledComponentArray.AddMember("Active", activeComponent.active, allocator);
				componentArray.AddMember("ActiveComponent", enabledComponentArray, allocator);
			}
			if (entity.HasComponent<HierarchyComponent>())
			{
				rapidjson::Value hierarchyComponentArray(rapidjson::kObjectType);
				hierarchyComponentArray.AddMember("ParentUUID", hierarchyComponent.parentID, allocator);
				componentArray.AddMember("HierarchyComponent", hierarchyComponentArray, allocator);
			} // TODO: think about moving these to something like "UUIDComponent::GUID": or "UUIDComponent_GUID": instead of this whole nesting stuff

			if (entity.HasComponent<TransformComponent>())
			{
				rapidjson::Value transformComponentArray(rapidjson::kObjectType);

				transformComponentArray.AddMember("PositionX", transformComponent.transform.position.x, allocator);
				transformComponentArray.AddMember("PositionY", transformComponent.transform.position.y, allocator);
				transformComponentArray.AddMember("PositionZ", transformComponent.transform.position.z, allocator);

				transformComponentArray.AddMember("RotationX", transformComponent.transform.rotation.x, allocator);
				transformComponentArray.AddMember("RotationY", transformComponent.transform.rotation.y, allocator);
				transformComponentArray.AddMember("RotationZ", transformComponent.transform.rotation.z, allocator);
				transformComponentArray.AddMember("RotationW", transformComponent.transform.rotation.w, allocator);

				transformComponentArray.AddMember("ScaleX", transformComponent.transform.scale.x, allocator);
				transformComponentArray.AddMember("ScaleY", transformComponent.transform.scale.y, allocator);
				transformComponentArray.AddMember("ScaleZ", transformComponent.transform.scale.z, allocator);

				transformComponentArray.AddMember("RotationEulerAnglesHintX", transformComponent.transform.eulerAnglesHint.x, allocator);
				transformComponentArray.AddMember("RotationEulerAnglesHintY", transformComponent.transform.eulerAnglesHint.y, allocator);
				transformComponentArray.AddMember("RotationEulerAnglesHintZ", transformComponent.transform.eulerAnglesHint.z, allocator);

				componentArray.AddMember("TransformComponent", transformComponentArray, allocator);
			}

			if (entity.HasComponent<CameraComponent>())
			{
				const auto& cameraComponent = entity.GetComponent<CameraComponent>();

				rapidjson::Value cameraComponentObject(rapidjson::kObjectType);

				cameraComponentObject.AddMember("ProjectionType", (int64)cameraComponent.camera.GetProjectionType(), allocator);

				cameraComponentObject.AddMember("PerspectiveFOV", cameraComponent.camera.GetPerspectiveVerticalFOV(), allocator);
				cameraComponentObject.AddMember("PerspectiveNear", cameraComponent.camera.GetPerspectiveNearClip(), allocator);
				cameraComponentObject.AddMember("PerspectiveFar", cameraComponent.camera.GetPerspectiveFarClip(), allocator);

				cameraComponentObject.AddMember("OrthographicSize", cameraComponent.camera.GetOrthographicSize(), allocator);
				cameraComponentObject.AddMember("OrthographicNear", cameraComponent.camera.GetOrthographicNearClip(), allocator);
				cameraComponentObject.AddMember("OrthographicFar", cameraComponent.camera.GetOrthographicFarClip(), allocator);


				cameraComponentObject.AddMember("ClearColorR", cameraComponent.camera.clearColor.r, allocator);
				cameraComponentObject.AddMember("ClearColorG", cameraComponent.camera.clearColor.g, allocator);
				cameraComponentObject.AddMember("ClearColorB", cameraComponent.camera.clearColor.b, allocator);
				cameraComponentObject.AddMember("ClearColorA", cameraComponent.camera.clearColor.a, allocator);

				cameraComponentObject.AddMember("ViewportSizeX", cameraComponent.camera.viewportSize.x, allocator);
				cameraComponentObject.AddMember("ViewportSizeY", cameraComponent.camera.viewportSize.y, allocator);

				cameraComponentObject.AddMember("ViewportOffsetX", cameraComponent.camera.viewportOffset.x, allocator);
				cameraComponentObject.AddMember("ViewportOffsetY", cameraComponent.camera.viewportOffset.y, allocator);

				componentArray.AddMember("CameraComponent", cameraComponentObject, allocator);
			}

			entityObject.AddMember("Components", componentArray, allocator);
			entityArray.PushBack(entityObject, allocator);
		}
		//for (const auto& obj : m_Properties)
		//{
		//	rapidjson::Value jsonObject(rapidjson::kObjectType);
		//
		//	jsonObject.AddMember("PropertyName", rapidjson::Value(obj.name.c_str(), allocator), allocator);
		//	jsonObject.AddMember("PropertyFormat", (int64)obj.propertyFormat, allocator);
		//
		//	//doc.PushBack(jsonObject, allocator);
		//	objArray.PushBack(jsonObject, allocator);
		//}
		doc.AddMember("Entities", entityArray, allocator);
	}

    SceneSerializer::SceneSerializer(Scene* scene)
        : m_Scene(scene)
    {
    }

    void SceneSerializer::Serialize(const std::string& filePath)
    {
		std::string source = JSON::SerializeObject(*m_Scene, JSON::SerializerOptions(true));

		//std::string filePath("Assets/Scenes/");
		//filePath.append(m_Scene->m_Name);
		//filePath.append(".ctscene"); // .cts, .ctscene, .ctenv

		std::ofstream outFile(filePath);
		CT_CORE_ASSERT(outFile.is_open(), "Unable to open scene file for writing!");
		outFile << source;
		outFile.close();
    }

    bool SceneSerializer::Deserialize(const std::string& filePath)
    {
		bool status = false;

        return status;
    }
}