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

        virtual void StartAsyncLoad(const std::string& filepath) override;
        virtual void LoadDataInAsset() override;

    protected:
        void AsyncLoad(std::string filepath);

    private:
        SMeshAssetData m_meshAssetData;
    };
}
