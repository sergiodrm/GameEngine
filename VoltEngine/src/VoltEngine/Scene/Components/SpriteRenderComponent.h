#pragma once
#include "glm/vec4.hpp"
#include "VoltEngine/Scene/Component.h"


namespace Volt
{
    class CSpriteRenderComponent : public CComponent
    {
    DECLARE_DERIVED_CLASS(CSpriteRenderComponent, CComponent)
    public:
        CSpriteRenderComponent(const glm::vec4& color = glm::vec4(1.f))
            : m_color(color) {}

        const glm::vec4& GetColor() const { return m_color; }
        void SetColor(const glm::vec4& color) { m_color = color; }

    private:
        glm::vec4 m_color;
    };
}
