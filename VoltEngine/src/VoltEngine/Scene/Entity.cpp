#include "Entity.h"

#include "Scene.h"
#include "Components/CameraComponent.h"
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
