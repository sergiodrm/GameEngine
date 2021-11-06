#pragma once
#include "VoltEngine/Scene/Component.h"
#include "glm/glm.hpp"

namespace Volt
{
    class CTransformComponent : public CComponent
    {
    DECLARE_DERIVED_CLASS(CComponent)
    public:
        glm::mat4 GetTransform() const;

    private:
        glm::vec3 m_position {0.f};
        glm::vec3 m_rotation {0.f};
        glm::vec3 m_scale {1.f};
    };
}
