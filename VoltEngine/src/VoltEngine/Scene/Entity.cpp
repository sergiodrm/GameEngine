#include "Entity.h"

#include "Scene.h"
#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"
#include "Components/MeshComponent.h"
#include "Components/NativeScriptComponent.h"

namespace Volt
{
    CEntity::CEntity(CScene* scene)
        : m_sceneContext(scene), m_id(0), m_status(EEntityStatus::Active)
    {
        VOLT_ASSERT(m_sceneContext, "Entity was created without scene context!");
    }


    CEntity::~CEntity()
    {
        Destroy();
    }

    void CEntity::Destroy()
    {
        m_status = EEntityStatus::Destroyed;
        DestroyComponents();
        VOLT_LOG(Trace, "Entity [ID: {0}] was destroyed.", GetID());
    }

    void CEntity::OnComponentAdded(CComponent* component)
    {
        CNativeScriptComponent* nativeScript = dynamic_cast<CNativeScriptComponent*>(component);
        if (nativeScript)
        {
            nativeScript->OnCreate();
        }
        else
        {
            CCameraComponent* cameraComponent = dynamic_cast<CCameraComponent*>(component);
            if (cameraComponent)
            {
                cameraComponent->GetCamera().SetViewportSize(m_sceneContext->GetViewportWidth(),
                                                             m_sceneContext->GetViewportHeight());
            }
            else
            {
                CLightComponent* lightComponent = dynamic_cast<CLightComponent*>(component);
                if (lightComponent)
                {
                    m_sceneContext->AddLight(this);

                    //// Debug mesh
                    //static SharedPtr<IMesh> cube = CMeshManager::Get().Load("assets/models/cube/basic_cube.obj");
                    //AddComponent<CMeshComponent>(cube);
                }
            }
        }
#ifdef VOLT_DEBUG
        VOLT_LOG(Trace, "Adding component {0} from entity {1}.", component->GetTypeName().c_str(), GetID());
#endif
    }

    void CEntity::OnComponentRemoved(CComponent* component)
    {
        CNativeScriptComponent* nativeScript = dynamic_cast<CNativeScriptComponent*>(component);
        if (nativeScript)
        {
            nativeScript->OnDestroy();
        }
        else
        {
            CLightComponent* lightComponent = dynamic_cast<CLightComponent*>(component);
            if (lightComponent)
            {
                m_sceneContext->RemoveLight(this);
            }
        }
#ifdef VOLT_DEBUG
        VOLT_LOG(Trace, "Removing component {0} from entity {1}.", component->GetTypeName().c_str(), GetID());
#endif
    }

    void CEntity::DestroyComponents()
    {
        for (CComponent* it : m_components)
        {
            OnComponentRemoved(it);
            delete it;
        }
        m_components.clear();
    }
}
