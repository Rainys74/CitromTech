#include "Scene.h"

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
        return Entity(m_SceneRegistry.create(), this);
    }
    void Scene::DestroyEntity(Entity entity)
    {
    }
}