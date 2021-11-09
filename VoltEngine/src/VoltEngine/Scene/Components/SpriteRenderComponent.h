#pragma once
#include "glm/vec4.hpp"
#include "VoltEngine/Renderer/TextureLoader.h"
#include "VoltEngine/Scene/Component.h"


namespace Volt
{
    class ITexture;

    class CSpriteRenderComponent : public CComponent
    {
    DECLARE_DERIVED_CLASS(CSpriteRenderComponent, CComponent)
    public:
        CSpriteRenderComponent(const Ref<ITexture>& texture = nullptr, const glm::vec4& color = glm::vec4(1.f))
            : m_texture(texture), m_color(color) {}

        CSpriteRenderComponent(const std::string& textureFilePath = {}, const glm::vec4& color = glm::vec4(1.f))
            : m_color(color)
        {
            m_texture = textureFilePath.empty() ? nullptr : CTextureLoader::Load(textureFilePath);
        }

        const Ref<ITexture>& GetTexture() const { return m_texture; }
        void SetTexture(const Ref<ITexture>& texture) { m_texture = texture; }

        const glm::vec4& GetColor() const { return m_color; }
        void SetColor(const glm::vec4& color) { m_color = color; }

    private:
        Ref<ITexture> m_texture;
        glm::vec4 m_color;
    };
}
