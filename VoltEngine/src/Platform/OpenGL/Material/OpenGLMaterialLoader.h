#pragma once
#include "glm/vec4.hpp"
#include "VoltEngine/AssetManager/AssetLoader.h"

namespace Volt
{
    struct SMaterialAssetData
    {
        std::string MaterialName {};
        glm::vec4 Ambient {1.f};
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

        virtual void LoadDataInAsset() override
        {
            /* Empty */
        }

    protected:
        virtual void Load(std::string filepath, bool async) override
        {
            /* Empty */
        }
    };
}
