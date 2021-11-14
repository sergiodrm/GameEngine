#pragma once
#include "VoltEngine/Renderer/VertexArray.h"

namespace Volt
{
    class COpenGLVertexArray : public IVertexArray
    {
    public:
        COpenGLVertexArray();
        virtual ~COpenGLVertexArray() override;

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void AddVertexBuffer(const SharedPtr<IVertexBuffer>& vertexBuffer) override;
        virtual void SetIndexBuffer(const SharedPtr<IIndexBuffer>& indexBuffer) override;

        virtual const std::vector<SharedPtr<IVertexBuffer>>& GetVertexBuffer() const override { return m_vertexBuffers; }
        virtual const SharedPtr<IIndexBuffer>& GetIndexBuffer() const override { return m_indexBuffer; }

    private:
        uint32_t m_rendererID;
        std::vector<SharedPtr<IVertexBuffer>> m_vertexBuffers;
        SharedPtr<IIndexBuffer> m_indexBuffer;
    };
}
