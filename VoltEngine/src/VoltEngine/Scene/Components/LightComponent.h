#pragma once
#include <glm/glm.hpp>
#include "VoltEngine/Scene/Component.h"

namespace Volt
{
    enum class ELightType
    {
        Point,
        Directional,
        Max
    };

    struct SLight
    {
        ELightType Type;
        glm::vec3 Position;
        glm::vec4 Color;
    };

    class CLightComponent : public CComponent
    {
    DECLARE_DERIVED_CLASS(CLightComponent, CComponent);
    public:
        CLightComponent() = default;
        const ELightType& GetType() const { return m_type; }
        void SetType(ELightType type) { m_type = type; }
        const glm::vec4& GetColor() const { return m_color; }
        void SetColor(const glm::vec4& color) { m_color = color; }

    private:
        ELightType m_type {ELightType::Point};
        glm::vec4 m_color {1.f};
    };
}
