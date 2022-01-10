#pragma once
#include "glm/vec4.hpp"
#include "VoltEngine/AssetManager/AssetLoader.h"

namespace Volt
{
    struct SMaterialAssetData
    {
        glm::vec4 Ambient;
        glm::vec4 Diffuse;
        glm::vec4 Specular;
        float Shininess {0.f};

        std::string TextureFilepath;
    };

    class COpenGLMaterialLoader : public IAssetLoader
    {
    public:
        COpenGLMaterialLoader(IAsset* asset)
            : IAssetLoader(asset) {}

        virtual void StartAsyncLoad(const std::string& filepath) override;
        virtual void LoadDataInAsset() override;
    };
}
