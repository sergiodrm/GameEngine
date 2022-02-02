#include "OpenGLTexture2DLoader.h"

#include <future>

#include "OpenGLTexture2D.h"
#include "stb_image.h"
#include "VoltEngine/AssetManager/AssetManager.h"
#include "VoltEngine/Core/Time.h"

namespace Volt
{
    COpenGLTexture2DLoader::COpenGLTexture2DLoader(IAsset* asset)
        : IAssetLoader(asset) {}

    void COpenGLTexture2DLoader::LoadDataInAsset()
    {
        if (COpenGLTexture2D* textureAsset = dynamic_cast<COpenGLTexture2D*>(GetAsset()))
        {
            textureAsset->LoadData(m_textureAssetData);
            stbi_image_free(m_textureAssetData.Data);
        }
    }

    void COpenGLTexture2DLoader::Load(std::string filepath, bool async)
    {
        PROFILE_SCOPE(Texture2DLoaderAsyncLoad);
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(filepath.c_str(), &m_textureAssetData.Width, &m_textureAssetData.Height, &m_textureAssetData.Channels, 0);
        m_textureAssetData.Data = data;
        VOLT_ASSERT(data, "Couldn't load image from: {0}", filepath.c_str());
        m_textureAssetData.TextureName = filepath;

        if (async)
        {
            CAssetManager::Get().PushLoadRequest(this);
        }
    }
}
