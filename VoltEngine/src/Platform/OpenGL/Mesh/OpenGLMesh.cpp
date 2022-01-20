#include "OpenGLMesh.h"

#include "VoltEngine/Renderer/Buffer.h"
#include "VoltEngine/Renderer/VertexData.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "OpenGLMeshLoader.h"
#include "tiny_obj_loader.h"
#include "VoltEngine/AssetManager/AssetManager.h"
#include "VoltEngine/Renderer/Material.h"
#include "VoltEngine/Renderer/VertexArray.h"

namespace Volt
{
    COpenGLMesh::~COpenGLMesh()
    {
        m_vertexArray.reset();
        m_material.reset();
    }

    IAssetLoader* COpenGLMesh::CreateLoader()
    {
        COpenGLMeshLoader* meshLoader = new COpenGLMeshLoader(this);
        return meshLoader;
    }

    bool COpenGLMesh::IsLoaded() const
    {
        return m_isLoaded;
    }

    const SharedPtr<IVertexArray>& COpenGLMesh::GetVertexArray() const { return m_vertexArray; }

    void COpenGLMesh::LoadData(const SMeshAssetData& data)
    {
        // Vertices
        const SVertexData* vertexDataPtr = data.Vertices.data();
        const float* verticesPtr = vertexDataPtr->GetData();
        SharedPtr<IVertexBuffer> vertexBuffer = IVertexBuffer::Create(verticesPtr, static_cast<uint32_t>(data.Vertices.size()) * sizeof(SVertexData));
        vertexBuffer->SetLayout(SVertexData::GetStaticBufferLayout());

        // Indices
        const SharedPtr<IIndexBuffer> indexBuffer = IIndexBuffer::Create(data.Indices.data(), static_cast<uint32_t>(data.Indices.size()));

        // Vertex Array
        m_vertexArray = IVertexArray::Create();
        m_vertexArray->AddVertexBuffer(vertexBuffer);
        m_vertexArray->SetIndexBuffer(indexBuffer);

        // Cache material data
        const SMaterialAssetData& materialData = data.MaterialAssetData;
        m_material = CAssetManager::CreateEmptyAsset<IMaterial>(materialData.MaterialName);
        m_material->SetAmbient(materialData.Ambient);
        m_material->SetSpecular(materialData.Specular);
        m_material->SetDiffuse(materialData.Diffuse);
        m_material->SetShininess(materialData.Shininess);
        m_material->SetTexture(materialData.TextureFilepath);
        m_isLoaded = true;
    }
}
