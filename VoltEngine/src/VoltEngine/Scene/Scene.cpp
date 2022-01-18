#include "Scene.h"

#include "Entity.h"
#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"
#include "Components/MeshComponent.h"
#include "Components/NativeScriptComponent.h"
#include "Components/SpriteRenderComponent.h"
#include "Components/TagComponent.h"
#include "Components/TransformComponent.h"
#include "VoltEngine/Renderer/EditorCamera.h"
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

    void CScene::OnUpdateRuntime(float elapsedSeconds)
    {
        RunEntitiesScripts(elapsedSeconds);
        CEntity* cameraEntity = GetPrimaryCamera();
        if (cameraEntity)
        {
            const CCameraComponent* cameraComponent = cameraEntity->GetComponent<CCameraComponent>();
            const CTransformComponent* transformComponent = cameraEntity->GetComponent<CTransformComponent>();
            RenderScene(cameraComponent->GetCamera().GetProjection(), inverse(transformComponent->GetTransform()));
        }
    }

    void CScene::OnUpdateEditor(const SharedPtr<CEditorCamera>& editorCamera)
    {
        RenderScene(editorCamera->GetProjection(), editorCamera->GetViewMatrix());
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


    void CScene::AddLight(CEntity* light)
    {
        m_sceneLights.push_back(light);
    }

    void CScene::RemoveLight(CEntity* light)
    {
        const std::vector<CEntity*>::iterator it = std::find(m_sceneLights.begin(), m_sceneLights.end(), light);
        if (it != m_sceneLights.end())
        {
            m_sceneLights.erase(it);
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

    void CScene::RenderScene(const glm::mat4& projection, const glm::mat4& view)
    {
        // -------------------------------------- Render 2D --------------------------------------
        {
            CRenderer2D::BeginScene(projection, view);

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
            std::vector<SLight> lights;
            for (const CEntity* it : m_sceneLights)
            {
                const CLightComponent* lightComponent = it->GetComponent<CLightComponent>();
                const CTransformComponent* transformComponent = it->GetComponent<CTransformComponent>();
                SLight light {ELightType::Point, transformComponent->GetPosition(), lightComponent->GetColor()};
                lights.push_back(light);
            }
            CRenderer3D::BeginScene(projection, view, lights);
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
