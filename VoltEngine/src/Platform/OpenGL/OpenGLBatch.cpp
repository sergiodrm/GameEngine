#include "OpenGLBatch.h"

#include "VoltEngine/Renderer/Buffer.h"
#include "VoltEngine/Renderer/RenderCommand.h"
#include "VoltEngine/Renderer/VertexArray.h"
#include "VoltEngine/Renderer/VertexData.h"

namespace Volt
{
    COpenGLBatch::COpenGLBatch(uint32_t maxNumTriangles)
        : m_maxNumTriangles(maxNumTriangles), m_usedVertices(0), m_usedIndices(0)
    {
        const uint32_t maxNumIndices = maxNumTriangles * 3;

        // Create Vertex buffer and configure its layout
        m_vertexBuffer = IVertexBuffer::Create(maxNumIndices * sizeof(SVertexData));
        m_vertexBuffer->SetLayout(SVertexData::GetStaticBufferLayout());

        // Create Index buffer
        m_indexBuffer = IIndexBuffer::Create(maxNumIndices);

        // Bind them to our vertex array object
        m_vertexArray = IVertexArray::Create();
        m_vertexArray->AddVertexBuffer(m_vertexBuffer);
        m_vertexArray->SetIndexBuffer(m_indexBuffer);

        // Create ram buffers
        m_cachedVerticesBase = new SVertexData[maxNumIndices];
        m_cachedIndicesBase = new uint32_t[maxNumIndices];
    }

    COpenGLBatch::~COpenGLBatch()
    {
        delete[] m_cachedIndicesBase;
        delete[] m_cachedVerticesBase;
        m_vertexBuffer.reset();
        m_indexBuffer.reset();
        m_vertexArray.reset();
    }

    void COpenGLBatch::AddTriangles(const std::vector<SVertexData>& vertices, const std::vector<uint32_t>& indices)
    {
        // Store vertices
        SVertexData* vertexDataPtr = GetVertexPtr();
        const uint32_t vertexBytes = static_cast<uint32_t>(vertices.size()) * sizeof(SVertexData);
        memcpy_s(vertexDataPtr, vertexBytes, vertices.data(), vertexBytes);

        // Store indices
        uint32_t* indexDataPtr = GetIndexPtr();
        const uint32_t indexBytes = static_cast<uint32_t>(indices.size()) * sizeof(uint32_t);
        memcpy_s(indexDataPtr, indexBytes, indices.data(), indexBytes);

        // Add counters
        m_usedVertices += static_cast<uint32_t>(vertices.size());
        m_usedIndices += static_cast<uint32_t>(indices.size());
    }

    bool COpenGLBatch::CanAddTriangles(uint32_t numTriangles) const
    {
        return m_usedIndices / 3 + numTriangles < m_maxNumTriangles;
    }

    bool COpenGLBatch::IsEmpty() const { return m_usedIndices == 0 && m_usedVertices == 0; }

    void COpenGLBatch::Render() const
    {
        const uint32_t vertexBytes = m_usedVertices * sizeof(SVertexData);
        m_vertexBuffer->SetData(m_cachedVerticesBase, vertexBytes);
        const uint32_t indexBytes = m_usedIndices * sizeof(uint32_t);
        m_indexBuffer->SetData(m_cachedIndicesBase, indexBytes);

        CRenderCommand::DrawIndexed(m_vertexArray, m_usedIndices);
    }

    void COpenGLBatch::Clear()
    {
        m_usedIndices = 0;
        m_usedVertices = 0;
    }

    SVertexData* COpenGLBatch::GetVertexPtr() const
    {
        return m_cachedVerticesBase + m_usedVertices;
    }

    uint32_t* COpenGLBatch::GetIndexPtr() const
    {
        return m_cachedIndicesBase + m_usedIndices;
    }
}
