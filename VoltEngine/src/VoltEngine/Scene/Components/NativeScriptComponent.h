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

    class CRotateScriptComponent : public CNativeScriptComponent
    {
    DECLARE_DERIVED_CLASS(CRotateScriptComponent, CNativeScriptComponent)
    public:
        virtual void OnCreate() override
        {
            m_ownerTransform = GetOwner()->GetComponent<CTransformComponent>();
        }

        virtual void OnUpdate(float elapsedSeconds) override
        {
            if (IInput::IsKeyPressed(KeyCodes::Space))
            {
                if (!m_spacePressed)
                {
                    m_autoRotationEnabled = !m_autoRotationEnabled;
                    m_spacePressed = true;
                }
            }
            else
                m_spacePressed = false;

            float rotateDirection = 1.f;
            if (!m_autoRotationEnabled)
            {
                rotateDirection = 0.f;
                if (IInput::IsKeyPressed(KeyCodes::Q))
                    rotateDirection = -1.f;
                else if (IInput::IsKeyPressed(KeyCodes::E))
                    rotateDirection = 1.f;
            }
            const float rotation = elapsedSeconds * m_rotationSpeed * rotateDirection;
            glm::vec3 currentRotation = m_ownerTransform->GetRotation();
            currentRotation.y += glm::radians(rotation);
            currentRotation.y = fmodf(currentRotation.y, 2.f * 3.1415f);
            m_ownerTransform->SetRotation(currentRotation);
        }

    private:
        bool m_autoRotationEnabled {true};
        bool m_spacePressed {false};
        float m_rotationSpeed {20.f};
        CTransformComponent* m_ownerTransform {nullptr};
    };
}
