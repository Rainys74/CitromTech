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

	private:
		entt::registry m_SceneRegistry;

		friend class Entity;
	};
}