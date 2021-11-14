#pragma once
#include "glm/vec4.hpp"
#include "VoltEngine/Scene/Component.h"


namespace Volt
{
    class ITexture;

    class CSpriteRenderComponent : public CComponent
    {
    DECLARE_DERIVED_CLASS(CSpriteRenderComponent, CComponent)
    public:
        CSpriteRenderComponent(ITexture* texture = nullptr, const glm::vec4& color = glm::vec4(1.f))
            : m_texture(texture), m_color(color) {}

        const ITexture* GetTexture() const { return m_texture; }
        void SetTexture(ITexture* texture) { m_texture = texture; }

        const glm::vec4& GetColor() const { return m_color; }
        void SetColor(const glm::vec4& color) { m_color = color; }

    private:
        ITexture* m_texture;
        glm::vec4 m_color;
    };
}
