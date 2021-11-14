#pragma once

#include "glm/vec2.hpp"
#include "VoltEngine/Core/Core.h"

namespace Volt
{
    class ITexture;

    class CSubTexture
    {
    public:
        static SharedPtr<CSubTexture> Create(ITexture* texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = {1.f, 1.f});

        CSubTexture(ITexture* texture, const glm::vec2& min, const glm::vec2& max);

        const glm::vec2* GetCoords() const { return m_uvCoords; }

        const ITexture* GetTexture() const { return m_texture; }
        ITexture* GetTexture() { return m_texture; }

    private:
        ITexture* m_texture;
        glm::vec2 m_uvCoords[4];
    };
}
