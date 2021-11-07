#include "CameraComponent.h"

#include "VoltEngine/Scene/Entity.h"
#include "VoltEngine/Scene/Scene.h"

namespace Volt
{
    CCameraComponent::CCameraComponent(CEntity* owner)
        : CComponent(owner)
    {
        const CScene* scene = GetOwner()->GetSceneContext();
        m_camera.SetViewportSize(scene->GetViewportWidth(), scene->GetViewportHeight());
    }
}
