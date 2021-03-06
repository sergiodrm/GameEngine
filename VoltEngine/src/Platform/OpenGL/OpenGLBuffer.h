#pragma once
#include "VoltEngine/Renderer/Buffer.h"

namespace Volt
{
    class COpenGLVertexBuffer : public IVertexBuffer
    {
    public:
        COpenGLVertexBuffer(const float* vertices, uint32_t size);
        COpenGLVertexBuffer(uint32_t size);
        virtual ~COpenGLVertexBuffer() override;

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual const CBufferLayout& GetLayout() const override { return m_layout; }
        virtual void SetLayout(const CBufferLayout& layout) override { m_layout = layout; }

        virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
    private:
        CBufferLayout m_layout;
        uint32_t m_rendererID;
    };

    class COpenGLIndexBuffer : public IIndexBuffer
    {
    public:
        COpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
        COpenGLIndexBuffer(uint32_t count);
        virtual ~COpenGLIndexBuffer() override;

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual uint32_t GetCount() const override { return m_count; }

        virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
    private:
        uint32_t m_rendererID;
        uint32_t m_count;
    };
}
