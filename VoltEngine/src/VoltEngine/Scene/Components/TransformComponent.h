#pragma once
#include "VoltEngine/Scene/Component.h"
#include "glm/glm.hpp"

namespace Volt
{
    class CTransformComponent : public CComponent
    {
    DECLARE_DERIVED_CLASS(CComponent)
    public:
        CTransformComponent(CEntity* owner);

        glm::mat4 GetTransform() const;

        void SetPosition(const glm::vec3& position) { m_position = position; }
        void SetRotation(const glm::vec3& rotation) { m_rotation = rotation; }
        void SetScale(const glm::vec3& scale) { m_scale = scale; }

    private:
        glm::vec3 m_position {0.f};
        glm::vec3 m_rotation {0.f};
        glm::vec3 m_scale {1.f};
    };
}
