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

        virtual void AddVertexBuffer(const Ref<IVertexBuffer>& vertexBuffer) override;
        virtual void SetIndexBuffer(const Ref<IIndexBuffer>& indexBuffer) override;

        virtual const std::vector<Ref<IVertexBuffer>>& GetVertexBuffer() const override { return m_vertexBuffers; }
        virtual const Ref<IIndexBuffer>& GetIndexBuffer() const override { return m_indexBuffer; }

    private:
        uint32_t m_rendererID;
        std::vector<Ref<IVertexBuffer>> m_vertexBuffers;
        Ref<IIndexBuffer> m_indexBuffer;
    };
}
