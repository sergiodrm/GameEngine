#include "OpenGLMesh.h"

#include "OpenGLVertexArray.h"
#include "VoltEngine/Renderer/Buffer.h"
#include "VoltEngine/Renderer/VertexData.h"

namespace Volt
{
    COpenGLMesh::COpenGLMesh(const std::string& filepath)
    {
        // TODO
    }

    COpenGLMesh::COpenGLMesh(const std::vector<SVertexData>& vertices, const std::vector<uint32_t>& indices)
        : m_vertexData(vertices), m_indexData(indices)
    {
        // Vertices
        const SVertexData* vertexDataPtr = vertices.data();
        const float* verticesPtr = vertexDataPtr->GetData();
        SharedPtr<IVertexBuffer> vertexBuffer = IVertexBuffer::Create(verticesPtr, vertices.size() * sizeof(SVertexData));
        vertexBuffer->SetLayout(SVertexData::GetStaticBufferLayout());

        // Indices
        const SharedPtr<IIndexBuffer> indexBuffer = IIndexBuffer::Create(indices.data(), indices.size());

        // Vertex Array
        m_vertexArray = IVertexArray::Create();
        m_vertexArray->AddVertexBuffer(vertexBuffer);
        m_vertexArray->SetIndexBuffer(indexBuffer);
    }

    const SharedPtr<IVertexArray>& COpenGLMesh::GetVertexArray() const { return m_vertexArray; }
}
