#pragma once

#include "glm/vec2.hpp"
#include "VoltEngine/Core/Core.h"

namespace Volt
{
    class ITexture;

    class CSubTexture
    {
    public:
        static SharedPtr<CSubTexture> Create(const SharedPtr<ITexture>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = {1.f, 1.f});

        CSubTexture(const SharedPtr<ITexture>& texture, const glm::vec2& min, const glm::vec2& max);

        const glm::vec2* GetCoords() const { return m_uvCoords; }

        const SharedPtr<ITexture>& GetTexture() const { return m_texture; }

    private:
        SharedPtr<ITexture> m_texture;
        glm::vec2 m_uvCoords[4];
    };
}
