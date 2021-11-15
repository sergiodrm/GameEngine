#include "Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Volt
{
    SharedPtr<IVertexBuffer> IVertexBuffer::Create(const float* vertices, uint32_t size)
    {
        // @todo renderer API
        return CreateSharedPtr<COpenGLVertexBuffer>(vertices, size);
    }

    SharedPtr<IVertexBuffer> IVertexBuffer::Create(uint32_t size)
    {
        // @todo renderer API
        return CreateSharedPtr<COpenGLVertexBuffer>(size);
    }

    SharedPtr<IIndexBuffer> IIndexBuffer::Create(const uint32_t* indices, uint32_t count)
    {
        // @todo rendererAPI
        return CreateSharedPtr<COpenGLIndexBuffer>(indices, count);
    }
}
