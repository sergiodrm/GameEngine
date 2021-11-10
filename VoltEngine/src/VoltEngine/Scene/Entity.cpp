#include "Entity.h"

#include "Scene.h"
#include "Components/CameraComponent.h"
#include "Components/NativeScriptComponent.h"

namespace Volt
{
    CEntity::CEntity(CScene* scene)
        : m_sceneContext(scene), m_id(0)
    {
        VOLT_ASSERT(m_sceneContext, "Entity was created without scene context!");
    }


    CEntity::~CEntity()
    {
        for (CComponent* it : m_components)
        {
            OnComponentRemoved(it);
            delete it;
        }
        m_components.clear();
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
    }

    void CEntity::OnComponentRemoved(CComponent* component)
    {
        CNativeScriptComponent* nativeScript = dynamic_cast<CNativeScriptComponent*>(component);
        if (nativeScript)
        {
            nativeScript->OnDestroy();
        }
    }
}
