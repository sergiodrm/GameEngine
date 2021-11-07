#include "Scene.h"

#include "Entity.h"
#include "Components/TagComponent.h"
#include "Components/TransformComponent.h"

namespace Volt
{
    Ref<CEntity> CScene::CreateEntity(const std::string& name)
    {
        Ref<CEntity> entity = m_registry.Create(this);
        entity->AddComponent<CTagComponent>(name);
        entity->AddComponent<CTransformComponent>();
        return entity;
    }

    void CScene::OnUpdate(float elapsedSeconds) {}
    void CScene::OnViewportResize(uint32_t width, uint32_t height) {}
}
