#pragma once

#include "Core.h"
#include "CitromUUID.h"

#include "Vendor/EnTT/entt.hpp"

namespace Citrom
{
	class Entity;
	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity();
		void DestroyEntity(Entity entity);

		template<typename... Components>
		auto GetAllEntitiesWith()
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

	private:
		entt::registry m_SceneRegistry;

		friend class Entity;
	};
}