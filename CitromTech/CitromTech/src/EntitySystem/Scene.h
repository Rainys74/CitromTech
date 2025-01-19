#pragma once

#include "Core.h"

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

	private:
		entt::registry m_SceneRegistry;

		friend class Entity;
	};
}