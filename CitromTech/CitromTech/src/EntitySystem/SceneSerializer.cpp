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
	//template<char Suffix0 = 'X', char Suffix1 = 'Y', char Suffix2 = 'Z'>
	//static void _SerializeVector3(rapidjson::Value* object, const float32 vec3[3], rapidjson::Document::AllocatorType& allocator) //, const Math::Vector3& vec3)
	//{
	//}
#define SERIALIZE_FLOAT2_SUFFIXED(OBJECT, FLOAT2, NAME, ALLOCATOR, SUFFIX0, SUFFIX1)					OBJECT.AddMember((NAME SUFFIX0), (FLOAT2)[0], ALLOCATOR); \
																										OBJECT.AddMember((NAME SUFFIX1), (FLOAT2)[1], ALLOCATOR);

#define SERIALIZE_FLOAT3_SUFFIXED(OBJECT, FLOAT3, NAME, ALLOCATOR, SUFFIX0, SUFFIX1, SUFFIX2)			OBJECT.AddMember((NAME SUFFIX0), (FLOAT3)[0], ALLOCATOR); \
																										OBJECT.AddMember((NAME SUFFIX1), (FLOAT3)[1], ALLOCATOR); \
																										OBJECT.AddMember((NAME SUFFIX2), (FLOAT3)[2], ALLOCATOR)

#define SERIALIZE_FLOAT4_SUFFIXED(OBJECT, FLOAT4, NAME, ALLOCATOR, SUFFIX0, SUFFIX1, SUFFIX2, SUFFIX3)	SERIALIZE_FLOAT3_SUFFIXED(OBJECT, FLOAT4, NAME, ALLOCATOR, SUFFIX0, SUFFIX1, SUFFIX2); \
																										OBJECT.AddMember((NAME SUFFIX3), (FLOAT4)[3], ALLOCATOR)

#define SERIALIZE_VECTOR2(OBJECT, FLOAT2, NAME, ALLOCATOR) SERIALIZE_FLOAT2_SUFFIXED(OBJECT, FLOAT2, NAME, ALLOCATOR, "X", "Y")
#define SERIALIZE_VECTOR3(OBJECT, FLOAT3, NAME, ALLOCATOR) SERIALIZE_FLOAT3_SUFFIXED(OBJECT, FLOAT3, NAME, ALLOCATOR, "X", "Y", "Z")
#define SERIALIZE_VECTOR4(OBJECT, FLOAT4, NAME, ALLOCATOR) SERIALIZE_FLOAT4_SUFFIXED(OBJECT, FLOAT4, NAME, ALLOCATOR, "X", "Y", "Z", "W")

#define SERIALIZE_COLORF32X4(OBJECT, FLOAT4, NAME, ALLOCATOR) SERIALIZE_FLOAT4_SUFFIXED(OBJECT, FLOAT4, NAME, ALLOCATOR, "R", "G", "B", "A")

#define SERIALIZE_QUATERNION(OBJECT, FLOAT4, NAME, ALLOCATOR) SERIALIZE_VECTOR4(OBJECT, FLOAT4, NAME, ALLOCATOR)

	// Serialization
	void Scene::SerializeJson(rapidjson::Document& doc, rapidjson::Document::AllocatorType& allocator)
	{
		doc.SetObject();

		JSON_WRITER_SET_STRING("SceneName", GetSceneName());

		//doc.SetArray();

		rapidjson::Value entityArray(rapidjson::kArrayType);
		for (const auto [entityID, uuidComponent] : m_SceneRegistry.storage<UUIDComponent>().each())
		{
			Entity entity = Entity(entityID, this);

			const auto& nameComponent = entity.GetComponent<NameComponent>();
			const auto& activeComponent = entity.GetComponent<ActiveComponent>();
			const auto& hierarchyComponent = entity.GetComponent<HierarchyComponent>();

			rapidjson::Value entityObject(rapidjson::kObjectType);
			entityObject.AddMember("EntityName", rapidjson::Value(nameComponent.name.c_str(), allocator), allocator);
			//entityObject.AddMember("UUID", (uint64)uuidComponent.id, allocator);

			rapidjson::Value componentArray(rapidjson::kObjectType);

			//if (entity.HasComponent<UUIDComponent>())
			{
				rapidjson::Value guidComponentArray(rapidjson::kObjectType);
				guidComponentArray.AddMember("GUID", (uint64)uuidComponent.id, allocator);
				componentArray.AddMember("UUIDComponent", guidComponentArray, allocator);
			}
			//if (entity.HasComponent<NameComponent>())
			{
				rapidjson::Value tagComponentArray(rapidjson::kObjectType);
				tagComponentArray.AddMember("Name", rapidjson::Value(nameComponent.name.c_str(), allocator), allocator);
				componentArray.AddMember("NameComponent", tagComponentArray, allocator);
			}
			//if (entity.HasComponent<ActiveComponent>())
			{
				rapidjson::Value enabledComponentArray(rapidjson::kObjectType);
				enabledComponentArray.AddMember("Active", activeComponent.active, allocator);
				componentArray.AddMember("ActiveComponent", enabledComponentArray, allocator);
			}
			//if (entity.HasComponent<HierarchyComponent>()) // should hierarchy component be a must have or only for children?
			{
				rapidjson::Value hierarchyComponentArray(rapidjson::kObjectType);
				hierarchyComponentArray.AddMember("ParentUUID", hierarchyComponent.parentID, allocator);
				componentArray.AddMember("HierarchyComponent", hierarchyComponentArray, allocator);
			} // TODO: think about moving these to something like "UUIDComponent::GUID": or "UUIDComponent_GUID": instead of this whole nesting stuff

			if (entity.HasComponent<TransformComponent>())
			{
				const auto& transformComponent = entity.GetComponent<TransformComponent>();

				rapidjson::Value transformComponentArray(rapidjson::kObjectType);

				//transformComponentArray.AddMember("PositionX", transformComponent.transform.position.x, allocator);
				//transformComponentArray.AddMember("PositionY", transformComponent.transform.position.y, allocator);
				//transformComponentArray.AddMember("PositionZ", transformComponent.transform.position.z, allocator);
				SERIALIZE_VECTOR3(transformComponentArray, transformComponent.transform.position, "Position", allocator);

				//transformComponentArray.AddMember("RotationX", transformComponent.transform.rotation.x, allocator);
				//transformComponentArray.AddMember("RotationY", transformComponent.transform.rotation.y, allocator);
				//transformComponentArray.AddMember("RotationZ", transformComponent.transform.rotation.z, allocator);
				//transformComponentArray.AddMember("RotationW", transformComponent.transform.rotation.w, allocator);
				SERIALIZE_QUATERNION(transformComponentArray, transformComponent.transform.rotation, "Rotation", allocator);

				//transformComponentArray.AddMember("ScaleX", transformComponent.transform.scale.x, allocator);
				//transformComponentArray.AddMember("ScaleY", transformComponent.transform.scale.y, allocator);
				//transformComponentArray.AddMember("ScaleZ", transformComponent.transform.scale.z, allocator);
				SERIALIZE_VECTOR3(transformComponentArray, transformComponent.transform.scale, "Scale", allocator);

				//transformComponentArray.AddMember("RotationEulerAnglesHintX", transformComponent.transform.eulerAnglesHint.x, allocator);
				//transformComponentArray.AddMember("RotationEulerAnglesHintY", transformComponent.transform.eulerAnglesHint.y, allocator);
				//transformComponentArray.AddMember("RotationEulerAnglesHintZ", transformComponent.transform.eulerAnglesHint.z, allocator);
				SERIALIZE_VECTOR3(transformComponentArray, transformComponent.transform.eulerAnglesHint, "RotationEulerAnglesHint", allocator);

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


				//cameraComponentObject.AddMember("ClearColorR", cameraComponent.camera.clearColor.r, allocator);
				//cameraComponentObject.AddMember("ClearColorG", cameraComponent.camera.clearColor.g, allocator);
				//cameraComponentObject.AddMember("ClearColorB", cameraComponent.camera.clearColor.b, allocator);
				//cameraComponentObject.AddMember("ClearColorA", cameraComponent.camera.clearColor.a, allocator);
				SERIALIZE_COLORF32X4(cameraComponentObject, cameraComponent.camera.clearColor, "ClearColor", allocator);

				//cameraComponentObject.AddMember("ViewportSizeX", cameraComponent.camera.viewportSize.x, allocator);
				//cameraComponentObject.AddMember("ViewportSizeY", cameraComponent.camera.viewportSize.y, allocator);
				SERIALIZE_VECTOR2(cameraComponentObject, cameraComponent.camera.viewportSize, "ViewportSize", allocator);

				//cameraComponentObject.AddMember("ViewportOffsetX", cameraComponent.camera.viewportOffset.x, allocator);
				//cameraComponentObject.AddMember("ViewportOffsetY", cameraComponent.camera.viewportOffset.y, allocator);
				SERIALIZE_VECTOR2(cameraComponentObject, cameraComponent.camera.viewportOffset, "ViewportOffset", allocator);

				componentArray.AddMember("CameraComponent", cameraComponentObject, allocator);
			}

			if (entity.HasComponent<ModelComponent>())
			{
				const auto& modelComponent = entity.GetComponent<ModelComponent>();

				rapidjson::Value modelComponentObj(rapidjson::kObjectType);

				modelComponentObj.AddMember("MaterialName", "Placeholder", allocator);

				modelComponentObj.AddMember("CastShadows", modelComponent.castShadows, allocator);

				componentArray.AddMember("ModelComponent", modelComponentObj, allocator);
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

	// Deserialization
	/*namespace OnDemand
	{
		template<typename T>
		static Math::Vector3 DeserializeVector3(const T& constantElement) // const simdjson::dom::element& for non-demand // simdjson::simdjson_result<simdjson::fallback::ondemand::value>
		{
			auto& element = const_cast<T&>(constantElement);
			return Math::Vector3{
				element["X"].get<double>().value(), // element["X"].template get<double>().value(),
				element["Y"].get<double>().value(),
				element["Z"].get<double>().value()
			};
		}

		template<typename T>
		static Math::Quaternion DeserializeQuaternion(const T& constantElement)
		{
			auto& element = const_cast<T&>(constantElement);
			return Math::Quaternion(
				element["X"].get<double>().value(),
				element["Y"].get<double>().value(),
				element["Z"].get<double>().value(),
				element["W"].get<double>().value()
			);
		}

		template<typename T>
		static Math::ColorF32x4 DeserializeColorF32x4(const T& constantElement)
		{
			auto& element = const_cast<T&>(constantElement);
			return Math::ColorF32x4{
				element["R"].get<double>().value(),
				element["G"].get<double>().value(),
				element["B"].get<double>().value(),
				element["A"].get<double>().value()
			};
		}

		template<typename T>
		static Math::Vector2 DeserializeVector2(const T& constantElement)
		{
			auto& element = const_cast<T&>(constantElement);
			return Math::Vector2(
				element["X"].get<double>().value(),
				element["Y"].get<double>().value()
			);
		}
	}
	namespace NonDemand
	{
		/*static Math::Vector3 DeserializeVector3(const simdjson::dom::element& element)
		{
			double x; element["X"].get<double>(x);
			double y; element["Y"].get<double>(y);
			double z; element["Z"].get<double>(z);

			return Math::Vector3{
				(float32)x,
				(float32)y,
				(float32)z
			};
		}

		static Math::Quaternion DeserializeQuaternion(const simdjson::dom::element& element) 
		{
			double x = element["X"].get<double>().value();
			double y = element["Y"].get<double>().value();
			double z = element["Z"].get<double>().value();
			double w = element["W"].get<double>().value();

			return Math::Quaternion(
				(float32)x,
				(float32)y,
				(float32)z,
				(float32)w
			);
		}

		static Math::ColorF32x4 DeserializeColorF32x4(const simdjson::dom::element& element) 
		{
			//double r; element["R"].get<double>(r);
			//double g; element["G"].get<double>(g);
			//double b; element["B"].get<double>(b);
			//double a; element["A"].get<double>(a);
			//
			//return Math::ColorF32x4{
			//	(float32)r,
			//	(float32)g,
			//	(float32)b,
			//	(float32)a
			//};
		}

		static Math::Vector2 DeserializeVector2(const simdjson::dom::element& element) 
		{
			
		}* /
		static Math::Vector3 DeserializeVector3(const simdjson::dom::element& element)
		{
			return Math::Vector3{
				(float32)element["X"].get_double(),
				(float32)element["Y"].get_double(),
				(float32)element["Z"].get_double()
			};
		}

		static Math::Quaternion DeserializeQuaternion(const simdjson::dom::element& element)
		{
			return Math::Quaternion(
				(float32)element["X"].get_double(),
				(float32)element["Y"].get_double(),
				(float32)element["Z"].get_double(),
				(float32)element["W"].get_double()
			);
		}

		static Math::ColorF32x4 DeserializeColorF32x4(const simdjson::dom::element& element)
		{
			return Math::ColorF32x4{
				(float32)element["R"].get_double(),
				(float32)element["G"].get_double(),
				(float32)element["B"].get_double(),
				(float32)element["A"].get_double()
			};
		}

		static Math::Vector2 DeserializeVector2(const simdjson::dom::element& element)
		{
			return Math::Vector2{
				(float32)element["X"].get_double(),
				(float32)element["Y"].get_double()
			};
		}
	}*/
#define DESERIALIZE_FLOAT2_SUFFIXED(DOC, NAME, OUTFLOAT2, SUFFIX0, SUFFIX1)			 {	double _x, _y;					\
																						(DOC)[NAME SUFFIX0].get(_x);	\
																						(DOC)[NAME SUFFIX1].get(_y);	\
																														\
																						(OUTFLOAT2)[0] = _x;			\
																						(OUTFLOAT2)[1] = _y;			\
																					 }
#define DESERIALIZE_FLOAT3_SUFFIXED(DOC, NAME, OUTFLOAT3, SUFFIX0, SUFFIX1, SUFFIX2) {	double _x, _y, _z;				\
																						(DOC)[NAME SUFFIX0].get(_x);	\
																						(DOC)[NAME SUFFIX1].get(_y);	\
																						(DOC)[NAME SUFFIX2].get(_z);	\
																														\
																						(OUTFLOAT3)[0] = _x;			\
																						(OUTFLOAT3)[1] = _y;			\
																						(OUTFLOAT3)[2] = _z;			\
																					 }

#define DESERIALIZE_FLOAT4_SUFFIXED(DOC, NAME, OUTFLOAT4, SUFFIX0, SUFFIX1, SUFFIX2, SUFFIX3)	{	double _x, _y, _z, _w;			\
																									(DOC)[NAME SUFFIX0].get(_x);	\
																									(DOC)[NAME SUFFIX1].get(_y);	\
																									(DOC)[NAME SUFFIX2].get(_z);	\
																									(DOC)[NAME SUFFIX3].get(_w);	\
																																	\
																									(OUTFLOAT4)[0] = _x;			\
																									(OUTFLOAT4)[1] = _y;			\
																									(OUTFLOAT4)[2] = _z;			\
																									(OUTFLOAT4)[3] = _w;			\
																								}
#define DESERIALIZE_VECTOR2(DOC, NAME, OUTFLOAT2) DESERIALIZE_FLOAT2_SUFFIXED(DOC, NAME, OUTFLOAT2, "X", "Y")
#define DESERIALIZE_VECTOR3(DOC, NAME, OUTFLOAT3) DESERIALIZE_FLOAT3_SUFFIXED(DOC, NAME, OUTFLOAT3, "X", "Y", "Z")
#define DESERIALIZE_VECTOR4(DOC, NAME, OUTFLOAT4) DESERIALIZE_FLOAT4_SUFFIXED(DOC, NAME, OUTFLOAT4, "X", "Y", "Z", "W")

#define DESERIALIZE_QUATERNION(DOC, NAME, OUTFLOAT4) DESERIALIZE_VECTOR4(DOC, NAME, OUTFLOAT4)

#define DESERIALIZE_COLORF32X4(DOC, NAME, OUTFLOAT4) DESERIALIZE_FLOAT4_SUFFIXED(DOC, NAME, OUTFLOAT4, "R", "G", "B", "A")

    bool SceneSerializer::Deserialize(const std::string& filePath)
    {
		CT_PROFILE_MEMBER_FUNCTION();

		bool status = true;
		
		std::ifstream inFile(filePath);
		CT_CORE_ASSERT(inFile.is_open(), "Unable to open scene file for reading/parsing!");
		std::string jsonSrc((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());

		// On demand causes a hard-coded limit of 4 GB for parsing
#define USE_ON_DEMAND 1
#if USE_ON_DEMAND
		simdjson::ondemand::parser parser;
		simdjson::ondemand::document doc = parser.iterate(jsonSrc);
#else
		using namespace NonDemand;
		simdjson::dom::parser parser; // reallocates memory
		simdjson::dom::element doc = parser.parse(jsonSrc);
#endif

		std::string sceneName;
		JSON_READER_DOC_GET_STRING(doc, "SceneName", sceneName);
		m_Scene->RenameScene(sceneName);

		for (auto entityObject : doc["Entities"].get_array()) 
		{
			std::string entityName;
			uint64 entityUUID = 0;
			JSON_READER_DOC_GET_STRING(entityObject, "EntityName", entityName)
			//Entity entity = m_Scene->CreateEntity();

			auto components = entityObject["Components"].get_object();

			if (components["UUIDComponent"].error() == simdjson::SUCCESS) 
			{
				components["UUIDComponent"]["GUID"].get(entityUUID);
			}
			/*if (components["NameComponent"].error() == simdjson::SUCCESS)
			{
				components["NameComponent"]["Name"].get(entityName);
			}*/

			Entity entity = m_Scene->CreateEntityWithUUID(UUID(entityUUID), entityName);

			if (components["ActiveComponent"].error() == simdjson::SUCCESS) 
			{
				entity.GetComponent<ActiveComponent>().active = components["ActiveComponent"]["Active"].get_bool().value();
			}

			if (components["HierarchyComponent"].error() == simdjson::SUCCESS) 
			{
				entity.GetComponent<HierarchyComponent>().parentID = components["HierarchyComponent"]["ParentUUID"].get_uint64().value();
			}

			if (components["TransformComponent"].error() == simdjson::SUCCESS) 
			{
				auto&& transformComponentObj = components["TransformComponent"];

				TransformComponent& transformComponent = entity.GetComponent<TransformComponent>();

				DESERIALIZE_VECTOR3(transformComponentObj, "Position", transformComponent.transform.position);
				DESERIALIZE_QUATERNION(transformComponentObj, "Rotation", transformComponent.transform.rotation);
				DESERIALIZE_VECTOR3(transformComponentObj, "Scale", transformComponent.transform.scale);

				DESERIALIZE_VECTOR3(transformComponentObj, "RotationEulerAnglesHint", transformComponent.transform.eulerAnglesHint);

				//entity.AddComponent<TransformComponent>(transformComponent);
			}


			if (components["CameraComponent"].error() == simdjson::SUCCESS) 
			{
				auto&& cameraComponentObj = components["CameraComponent"];

				CameraComponent cameraComponent;

				cameraComponent.camera.SetProjectionType((Camera::ProjectionType)cameraComponentObj["ProjectionType"].get<int64>().value());

				cameraComponent.camera.SetPerspectiveVerticalFOV(cameraComponentObj["PerspectiveFOV"].get<double>().value());
				cameraComponent.camera.SetPerspectiveNearClip(cameraComponentObj["PerspectiveNear"].get<double>().value());
				cameraComponent.camera.SetPerspectiveFarClip(cameraComponentObj["PerspectiveFar"].get<double>().value());

				cameraComponent.camera.SetOrthographicSize(cameraComponentObj["OrthographicSize"].get<double>().value());
				cameraComponent.camera.SetOrthographicNearClip(cameraComponentObj["OrthographicNear"].get<double>().value());
				cameraComponent.camera.SetOrthographicFarClip(cameraComponentObj["OrthographicFar"].get<double>().value());

				//cameraComponent.camera.clearColor = DeserializeColorF32x4(components["CameraComponent"]["ClearColor"]);
				//cameraComponent.camera.viewportSize = DeserializeVector2(components["CameraComponent"]["ViewportSize"]);
				//cameraComponent.camera.viewportOffset = DeserializeVector2(components["CameraComponent"]["ViewportOffset"]);
				DESERIALIZE_COLORF32X4(cameraComponentObj, "ClearColor", cameraComponent.camera.clearColor);
				DESERIALIZE_VECTOR2(cameraComponentObj, "ViewportSize", cameraComponent.camera.viewportSize);
				DESERIALIZE_VECTOR2(cameraComponentObj, "ViewportOffset", cameraComponent.camera.viewportOffset);

				entity.AddComponent<CameraComponent>(cameraComponent);
			}
		}

        return status;
    }
}
