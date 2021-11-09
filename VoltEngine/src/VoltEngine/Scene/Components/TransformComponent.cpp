#include "TransformComponent.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/transform.hpp"
#include "glm/gtx/quaternion.hpp"

namespace Volt
{
    glm::mat4 CTransformComponent::GetTransform() const
    {
        return translate(glm::mat4(1.f), m_position)
            * toMat4(glm::quat(m_rotation))
            * scale(glm::mat4(1.f), m_scale);
    }
}
