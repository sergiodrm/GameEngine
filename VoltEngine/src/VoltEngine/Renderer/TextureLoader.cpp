#include "TextureLoader.h"

#include "Texture.h"

namespace Volt
{
    std::unordered_map<std::string, Ref<ITexture>> CTextureLoader::m_loadedTextures;

    Ref<ITexture> CTextureLoader::Load(const std::string& filePath)
    {
        Ref<ITexture> texture = nullptr;
        if (!filePath.empty())
        {
            texture = Find(filePath);
            if (!texture)
            {
                // Create
                texture = ITexture::Create(filePath);
            }
        }
        return texture;
    }

    bool CTextureLoader::IsLoaded(const std::string& filePath)
    {
        const std::unordered_map<std::string, Ref<ITexture>>::iterator it = m_loadedTextures.find(filePath);
        return it != m_loadedTextures.end();
    }

    Ref<ITexture> CTextureLoader::Find(const std::string& filePath)
    {
        Ref<ITexture> texture = nullptr;
        if (!filePath.empty())
        {
            const std::unordered_map<std::string, Ref<ITexture>>::iterator it = m_loadedTextures.find(filePath);
            if (it != m_loadedTextures.end())
            {
                texture = it->second;
            }
        }
        return texture;
    }

    void CTextureLoader::Unload(const std::string& filePath)
    {
        Ref<ITexture> texture = Find(filePath);
        if (texture)
        {
            texture.reset();
            m_loadedTextures.erase(filePath);
        }
    }
}
