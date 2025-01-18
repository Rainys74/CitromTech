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
        entity.AddComponent<TransformComponent>();

        return entity;
    }
    void Scene::DestroyEntity(Entity entity)
    {
    }
}