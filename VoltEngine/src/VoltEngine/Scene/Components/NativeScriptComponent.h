#pragma once
#include "TransformComponent.h"
#include "glm/glm.hpp"
#include "VoltEngine/Core/Input.h"
#include "VoltEngine/Core/KeyCodes.h"
#include "VoltEngine/Scene/Component.h"

namespace Volt
{
    class CTransformComponent;

    class CNativeScriptComponent : public CComponent
    {
    DECLARE_DERIVED_CLASS(CNativeScriptComponent, CComponent)
    public:
        virtual void OnCreate() {}
        virtual void OnUpdate(float elapsedSeconds) {}
        virtual void OnDestroy() {}
    };

    class CMovementScriptComponent : public CNativeScriptComponent
    {
    DECLARE_DERIVED_CLASS(CMovementScriptComponent, CNativeScriptComponent)
    public:
        virtual void OnCreate() override
        {
            CEntity* owner = GetOwner();
            m_ownerTransform = owner->GetComponent<CTransformComponent>();
            VOLT_ASSERT(m_ownerTransform, "Cannot get transform component from owner.");
        }

        virtual void OnUpdate(float elapsedSeconds) override
        {
            if (IInput::IsKeyPressed(KeyCodes::W))
            {
                m_direction.y += 1.f;
            }
            if (IInput::IsKeyPressed(KeyCodes::S))
            {
                m_direction.y -= 1.f;
            }
            if (IInput::IsKeyPressed(KeyCodes::A))
            {
                m_direction.x -= 1.f;
            }
            if (IInput::IsKeyPressed(KeyCodes::D))
            {
                m_direction.x += 1.f;
            }

            const glm::vec3 displacement = m_direction * m_speed;
            m_ownerTransform->SetPosition(displacement + m_ownerTransform->GetPosition());
            m_direction = {0.f, 0.f, 0.f};
        }

    private:
        glm::vec3 m_direction {0.f};
        float m_speed {1.f};
        CTransformComponent* m_ownerTransform {nullptr};
    };
}
