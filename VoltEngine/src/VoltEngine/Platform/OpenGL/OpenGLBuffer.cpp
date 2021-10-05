#include "OpenGLBuffer.h"
#include "glad/glad.h"

namespace Volt
{
    COpenGLVertexBuffer::COpenGLVertexBuffer(float* vertices, uint32_t size)
    {
        glCreateBuffers(1, &m_rendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    COpenGLVertexBuffer::COpenGLVertexBuffer(uint32_t size)
    {
        glCreateBuffers(1, &m_rendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    COpenGLVertexBuffer::~COpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &m_rendererID);
    }

    void COpenGLVertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
    }

    void COpenGLVertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void COpenGLVertexBuffer::SetData(const void* data, uint32_t size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    COpenGLIndexBuffer::COpenGLIndexBuffer(uint32_t* indices, uint32_t count)
        : m_rendererID(0), m_count(count)
    {
        glCreateBuffers(1, &m_rendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    COpenGLIndexBuffer::~COpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &m_rendererID);
    }

    void COpenGLIndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
    }

    void COpenGLIndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
