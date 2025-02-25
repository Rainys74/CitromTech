#pragma once

#include "Core.h"
#include "CitromUUID.h"

#include "Vendor/EnTT/entt.hpp"

#include "JSON/Writer.h"

namespace Citrom
{
	class Entity;
	class Scene
	{
	public:
		Scene(const std::string& name = "New Scene");
		~Scene();

		Entity CreateEntity();
		void DestroyEntity(Entity entity);

		inline void ClearScene()
		{
			m_SceneRegistry.clear();
		}

		template<typename... Components>
		auto GetAllEntitiesWith() const
		{
			return m_SceneRegistry.view<Components...>();
		}

		Entity GetByUUID(UUID uuid);

		Entity GetMainCameraEntity();
		
		/*template<typename T, typename... Args>
		void ForEachEntityCallback(Args&&... args)
		{
			m_Scene->m_Registry.each(std::forward<Args>(args)...);
		}*/
		/*template<typename T, typename... Args>
		void ForEachEntityCallback(void(*callback)(T&, Args&&...))
		{
			m_Scene->m_Registry.each([callback](auto entity) 
			{
				auto& component = m_Scene->m_Registry.get<T>(entity);
				callback(component, std::forward<Args>(args)...);
			});
		}*/

		FORCE_INLINE std::string GetSceneName() { return m_Name; }
	private:
		//void SerializeEntity(rapidjson::Document& doc, rapidjson::Document::AllocatorType& allocator)
	public:
		// Serialization
		void SerializeJson(rapidjson::Document& doc, rapidjson::Document::AllocatorType& allocator);
	private:
		entt::registry m_SceneRegistry;
		std::string m_Name;

		friend class Entity;
		friend class SceneSerializer;
	};
}