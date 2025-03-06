#pragma once

#include "Scene.h"
#include "Components/EssentialComponents.h"

#include "CitromUUID.h"

namespace Citrom
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity entityID, Scene* parentScene)
			: m_EntityID(entityID), m_Scene(parentScene) {}
		Entity(const Entity&) = default;
		~Entity() = default;

		inline void SetParent(Entity& parentEntity)
		{
			GetComponent<HierarchyComponent>().parentID = parentEntity.GetUUID(); // TODO: if i decide to make a HierarchyComponent optional, i should change these to use EnsureComponent instead of GetComponent
		}
		inline void SetChild(Entity& childEntity)
		{
			childEntity.GetComponent<HierarchyComponent>().parentID = this->GetUUID();
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return m_Scene->m_SceneRegistry.emplace<T>(m_EntityID, std::forward<Args>(args)...);
		}

		template<typename T>
		inline T& GetComponent()
		{
			return m_Scene->m_SceneRegistry.get<T>(m_EntityID);
		}

		template<typename T>
		FORCE_INLINE T& EnsureComponent() // GetOrAddComponent
		{
			if (HasComponent<T>())
				return GetComponent<T>();
			else
				return AddComponent<T>();
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

		//FORCE_INLINE operator bool() const { return m_EntityID != entt::null && m_Scene != nullptr; }

		FORCE_INLINE Scene* GetScene() { return m_Scene; }

		FORCE_INLINE UUID GetUUID()
		{
			return GetComponent<UUIDComponent>().id;
		}
		inline void RenameEntity(const std::string& name)
		{
			GetComponent<NameComponent>().name = name;
		}

		inline void SetActive(bool state = true)
		{
			GetComponent<ActiveComponent>().active = state;
		}
		FORCE_INLINE bool IsActive()
		{
			return GetComponent<ActiveComponent>().active;
		}

		FORCE_INLINE operator bool() const { return m_EntityID != entt::null; }
		FORCE_INLINE operator entt::entity() const { return m_EntityID; }
	private:
		entt::entity m_EntityID = entt::null;
		Scene* m_Scene = nullptr;
	};
}
