#include "Scene.h"
#include "Components/EssentialComponents.h"
#include "Components/RendererComponents.h"

#include "Entity.h"

namespace Citrom
{
    Scene::Scene(const std::string& name)
        : m_Name(name)
    {
    }
    /*Scene::Scene(const Scene& other)
    {
        const auto& reg = other.m_SceneRegistry;
        m_SceneRegistry.assign(reg.data(), reg.data() + reg.size(), reg.destroyed());
    }*/
    Scene::~Scene()
    {
    }

    Entity Scene::CreateEntity(const std::string& entityName)
    {
        return CreateEntityWithUUID(UUID(), entityName);
    }

    Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& entityName)
    {
        Entity entity = Entity(m_SceneRegistry.create(), this);

        entity.AddComponent<UUIDComponent>().id = uuid;
        entity.AddComponent<NameComponent>().name = entityName;
        entity.AddComponent<ActiveComponent>().active = true;
        entity.AddComponent<HierarchyComponent>();
        entity.AddComponent<TransformComponent>().transform.scale = Math::Vector3{ 1.0f, 1.0f, 1.0f };

        return entity;
    }
    void Scene::DestroyEntity(Entity entity)
    {
        m_SceneRegistry.destroy(entity);
    }
    Entity Scene::GetByUUID(UUID uuid)
    {
        auto view = GetAllEntitiesWith<UUIDComponent>(); //m_SceneRegistry.view<UUIDComponent>();
        for (auto entity : view)
        {
            auto& uuidComponent = view.get<UUIDComponent>(entity);
            if (uuidComponent.id == uuid)
            {
                return Entity(entity, this);
            }
        }

        return Entity(entt::null, this);
    }
    Entity Scene::GetMainCameraEntity()
    {
        auto view = m_SceneRegistry.view<CameraComponent>();
        for (auto entity : view)
        {
            if (!Entity(entity, this).GetComponent<ActiveComponent>().active)
                continue;

            const auto& cameraComponent = view.get<CameraComponent>(entity);

            return Entity(entity, this);
        }
        return Entity(entt::null, this);
    }
}