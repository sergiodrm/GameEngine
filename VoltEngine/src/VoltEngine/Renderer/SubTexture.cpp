#include "SubTexture.h"

#include "Texture.h"

Volt::Ref<Volt::CSubTexture> Volt::CSubTexture::Create(const Ref<ITexture>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize)
{
    const glm::vec2 min =
        {
            coords.x * cellSize.x / static_cast<float>(texture->GetWidth()),
            coords.y * cellSize.y / static_cast<float>(texture->GetHeight())
        };
    const glm::vec2 max =
        {
            (coords.x + spriteSize.x) * cellSize.x / static_cast<float>(texture->GetWidth()),
            (coords.y + spriteSize.y) * cellSize.y / static_cast<float>(texture->GetHeight())
        };
    return CreateRef<CSubTexture>(texture, min, max);
}

Volt::CSubTexture::CSubTexture(const Ref<ITexture>& texture, const glm::vec2& min, const glm::vec2& max)
    : m_texture(texture)
{
    m_uvCoords[0] = {min.x, min.y};
    m_uvCoords[1] = {max.x, min.y};
    m_uvCoords[2] = {max.x, max.y};
    m_uvCoords[3] = {min.x, max.y};
}
