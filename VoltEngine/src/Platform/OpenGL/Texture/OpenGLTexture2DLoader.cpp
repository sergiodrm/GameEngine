#include "OpenGLTexture2DLoader.h"

#include <future>

#include "OpenGLTexture2D.h"
#include "stb_image.h"
#include "VoltEngine/AssetManager/AssetManager.h"

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

    void COpenGLTexture2DLoader::StartAsyncLoad(const std::string& filepath)
    {
        std::future<void> future = std::async(std::launch::async, &COpenGLTexture2DLoader::AsyncLoad, this, filepath);
    }

    void COpenGLTexture2DLoader::AsyncLoad(std::string filepath)
    {
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(filepath.c_str(), &m_textureAssetData.Width, &m_textureAssetData.Height, &m_textureAssetData.Channels, 0);
        m_textureAssetData.Data = data;
        VOLT_ASSERT(data, "Couldn't load image from: {0}", filepath.c_str());
        m_textureAssetData.TextureName = filepath;
        CAssetManager::Get().PushLoadRequest(this);
    }
}
