#include "Scene.h"

#include "Entity.h"
#include "Components/CameraComponent.h"
#include "Components/SpriteRenderComponent.h"
#include "Components/TagComponent.h"
#include "Components/TransformComponent.h"
#include "VoltEngine/Renderer/Renderer.h"
#include "VoltEngine/Renderer/Renderer2D.h"

namespace Volt
{
    Ref<CEntity> CScene::CreateEntity(const std::string& name)
    {
        Ref<CEntity> entity = m_registry.Create(this);
        entity->AddComponent<CTagComponent>(name);
        entity->AddComponent<CTransformComponent>();
        return entity;
    }

    Ref<CEntity> CScene::GetPrimaryCamera() const
    {
        for (const Ref<CEntity>& it : m_registry)
        {
            const Ref<CCameraComponent> cameraComponent = it->GetComponent<CCameraComponent>();
            if (cameraComponent && cameraComponent->IsPrimary())
            {
                return it;
            }
        }
        return nullptr;
    }


    void CScene::OnUpdate(float elapsedSeconds)
    {
        // @todo Entity script code

        // Get scene camera
        const Ref<CEntity> cameraEntity = GetPrimaryCamera();
        if (cameraEntity)
        {
            const Ref<CCameraComponent> cameraComponent = cameraEntity->GetComponent<CCameraComponent>();
            const Ref<CTransformComponent> transformComponent = cameraEntity->GetComponent<CTransformComponent>();
            const glm::mat4 viewMatrix = transformComponent->GetTransform();
            CRenderer2D::BeginScene(cameraComponent->GetCamera(), viewMatrix);

            for (const Ref<CEntity>& it : m_registry)
            {
                const Ref<CTransformComponent> entityTransform = it->GetComponent<CTransformComponent>();
                const Ref<CSpriteRenderComponent> entityRender = it->GetComponent<CSpriteRenderComponent>();
                if (entityTransform && entityRender)
                {
                    CRenderer2D::DrawQuad(entityTransform->GetTransform(), entityRender->GetColor());
                }
            }

            CRenderer2D::EndScene();
        }
    }

    void CScene::OnViewportResize(uint32_t width, uint32_t height) {}
}
