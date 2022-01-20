#pragma once
#include "VoltEngine/AssetManager/AssetLoader.h"

namespace Volt
{
    struct STexture2DAssetData
    {
        std::string TextureName {};
        int32_t Width {0};
        int32_t Height {0};
        int32_t Channels {0};
        uint8_t* Data {nullptr};
    };

    class COpenGLTexture2DLoader : public IAssetLoader
    {
    public:
        COpenGLTexture2DLoader(IAsset* asset);

        virtual void LoadDataInAsset() override;
        virtual void StartAsyncLoad(const std::string& filepath) override;

    protected:
        void AsyncLoad(std::string filepath);
    private:
        STexture2DAssetData m_textureAssetData {};
    };
}
