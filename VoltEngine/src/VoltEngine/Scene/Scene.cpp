#include "Scene.h"

#include "Entity.h"
#include "Components/CameraComponent.h"
#include "Components/MeshComponent.h"
#include "Components/NativeScriptComponent.h"
#include "Components/SpriteRenderComponent.h"
#include "Components/TagComponent.h"
#include "Components/TransformComponent.h"
#include "VoltEngine/Renderer/Renderer.h"
#include "VoltEngine/Renderer/Renderer2D.h"
#include "VoltEngine/Renderer/Renderer3D.h"

namespace Volt
{
    CEntity* CScene::CreateEntity(const std::string& name)
    {
        CEntity* entity = m_registry.Create(this, name);
        return entity;
    }

    void CScene::RemoveEntity(CEntity& entity)
    {
        m_registry.DestroyEntity(&entity);
    }

    CEntity* CScene::GetPrimaryCamera() const
    {
        for (CEntity* it : m_registry)
        {
            const CCameraComponent* cameraComponent = it->GetComponent<CCameraComponent>();
            if (cameraComponent && cameraComponent->IsPrimary())
            {
                return it;
            }
        }
        return nullptr;
    }

    void CScene::OnUpdate(float elapsedSeconds)
    {
        RunEntitiesScripts(elapsedSeconds);
        RenderScene();
    }

    void CScene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_width = width;
        m_height = height;

        for (CEntity* it : m_registry)
        {
            CCameraComponent* cameraComponent = it->GetComponent<CCameraComponent>();
            if (cameraComponent)
            {
                cameraComponent->GetCamera().SetViewportSize(width, height);
            }
        }
    }


    void CScene::RunEntitiesScripts(float elapsedSeconds)
    {
        // Update scripts
        for (CEntity* it : m_registry)
        {
            CNativeScriptComponent* nativeScript = it->GetComponent<CNativeScriptComponent>();
            if (nativeScript)
            {
                nativeScript->OnUpdate(elapsedSeconds);
            }
        }
    }

    void CScene::RenderScene()
    {
        // Get scene camera
        const CEntity* cameraEntity = GetPrimaryCamera();
        if (cameraEntity)
        {
            const CCameraComponent* cameraComponent = cameraEntity->GetComponent<CCameraComponent>();
            const CTransformComponent* transformComponent = cameraEntity->GetComponent<CTransformComponent>();
            const glm::mat4 viewMatrix = transformComponent->GetTransform();

            // -------------------------------------- Render 2D --------------------------------------
            {
                CRenderer2D::BeginScene(cameraComponent->GetCamera(), viewMatrix);

                // Render entities
                for (CEntitiesRegistry::SIterator it(m_registry); it; ++it)
                {
                    CEntity* entity = *it;
                    const CTransformComponent* entityTransform = entity->GetComponent<CTransformComponent>();
                    const CSpriteRenderComponent* entitySpriteRenderComponent = entity->GetComponent<CSpriteRenderComponent>();
                    if (entityTransform && entitySpriteRenderComponent)
                    {
                        const SharedPtr<ITexture>& texture = entitySpriteRenderComponent->GetTexture();
                        if (texture)
                        {
                            CRenderer2D::DrawTexture(entityTransform->GetPosition(),
                                                     entityTransform->GetRotation(),
                                                     entityTransform->GetScale(),
                                                     texture,
                                                     entitySpriteRenderComponent->GetColor());
                        }
                        else
                        {
                            CRenderer2D::DrawQuad(entityTransform->GetTransform(), entitySpriteRenderComponent->GetColor());
                        }
                    }
                }

                CRenderer2D::EndScene();
            }
            // -------------------------------------- Render 3D --------------------------------------
            {
                CRenderer3D::BeginScene(cameraComponent->GetCamera(), viewMatrix);
                for (CEntitiesRegistry::SIterator it(m_registry); it; ++it)
                {
                    CEntity* entity = *it;
                    const CTransformComponent* entityTransform = entity->GetComponent<CTransformComponent>();
                    const CMeshComponent* entityMeshComponent = entity->GetComponent<CMeshComponent>();
                    if (entityTransform && entityMeshComponent)
                    {
                        const SharedPtr<IMesh>& mesh = entityMeshComponent->GetMesh();
                        if (mesh)
                        {
                            CRenderer3D::DrawMesh(entityTransform->GetTransform(), mesh);
                        }
                    }
                }
                CRenderer3D::EndScene();
            }
        }
    }
}
