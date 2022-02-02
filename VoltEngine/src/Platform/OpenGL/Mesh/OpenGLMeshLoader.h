#pragma once
#include <vector>

#include "Platform/OpenGL/Material/OpenGLMaterialLoader.h"
#include "VoltEngine/AssetManager/AssetLoader.h"
#include "VoltEngine/Renderer/VertexData.h"

namespace Volt
{
    struct SMeshAssetData
    {
        std::vector<SVertexData> Vertices;
        std::vector<uint32_t> Indices;

        SMaterialAssetData MaterialAssetData;
    };

    class COpenGLMeshLoader : public IAssetLoader
    {
    public:
        COpenGLMeshLoader(IAsset* asset);

        virtual void LoadDataInAsset() override;

    protected:
        virtual void Load(std::string filepath, bool async = false) override;

    private:
        SMeshAssetData m_meshAssetData;
    };
}
