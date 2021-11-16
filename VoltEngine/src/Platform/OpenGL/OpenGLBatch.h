#pragma once
#include "VoltEngine/Renderer/Batch.h"

namespace Volt
{
    class IVertexArray;
    class IIndexBuffer;
    class IVertexBuffer;

    class COpenGLBatch : public IBatch
    {
    public:
        COpenGLBatch(uint32_t maxNumTriangles);
        virtual ~COpenGLBatch() override;

        virtual void AddTriangles(
            const std::vector<SVertexData>& vertices,
            const std::vector<uint32_t>& indices,
            const SBatchConfig& batchConfig = {}) override;
        virtual bool CanAddTriangles(uint32_t numTriangles) const override;
        virtual bool IsEmpty() const override;
        virtual void Render() const override;
        virtual void Clear() override;
    protected:
        SVertexData* GetVertexPtr() const;
        uint32_t* GetIndexPtr() const;
    private:
        uint32_t m_maxNumTriangles;
        uint32_t m_usedVertices;
        uint32_t m_usedIndices;

        SharedPtr<IVertexArray> m_vertexArray;
        SharedPtr<IVertexBuffer> m_vertexBuffer;
        SharedPtr<IIndexBuffer> m_indexBuffer;

        SVertexData* m_cachedVerticesBase {nullptr};
        uint32_t* m_cachedIndicesBase {nullptr};
    };
}
