#include "Scene.h"
#include "Components/EssentialComponents.h"

#include "Entity.h"

namespace Citrom
{
    Scene::Scene()
    {
    }
    Scene::~Scene()
    {
    }
    Entity Scene::CreateEntity()
    {
        Entity entity = Entity(m_SceneRegistry.create(), this);

        entity.AddComponent<UUIDComponent>();
        entity.AddComponent<TransformComponent>().transform.scale = Math::Vector3{1.0f, 1.0f, 1.0f};

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
}