#pragma once

#include "Scene.h"
#include "Components/EssentialComponents.h"

#include "CitromUUID.h"

namespace Citrom
{
	class Entity
	{
	public:
		Entity(entt::entity entityID, Scene* parentScene)
			: m_EntityID(entityID), m_Scene(parentScene) {}
		Entity(const Entity&) = default;
		~Entity() = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return m_Scene->m_SceneRegistry.emplace<T>(m_EntityID, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			return m_Scene->m_SceneRegistry.get<T>(m_EntityID);
		}

		template<typename T>
		bool HasComponent()
		{
			//return m_Scene->m_SceneRegistry.all_of<T>(m_EntityID);
			return m_Scene->m_SceneRegistry.any_of<T>(m_EntityID);
		}

		template<typename T>
		void DestroyComponent()
		{
			m_Scene->m_SceneRegistry.remove<T>(m_EntityID);
		}

		FORCE_INLINE Scene* GetScene() { return m_Scene; }

		UUID UUID()
		{
			return GetComponent<UUIDComponent>().id;
		}

		FORCE_INLINE operator bool() const { return m_EntityID != entt::null; }
	private:
		entt::entity m_EntityID = entt::null;
		Scene* m_Scene = nullptr;
	};
}