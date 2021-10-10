#include "Buffer.h"

#include "VoltEngine/Platform/OpenGL/OpenGLBuffer.h"

namespace Volt
{
    Ref<IVertexBuffer> IVertexBuffer::Create(float* vertices, uint32_t size)
    {
        // @todo renderer API
        return CreateRef<COpenGLVertexBuffer>(vertices, size);
    }

    Ref<IVertexBuffer> IVertexBuffer::Create(uint32_t size)
    {
        // @todo renderer API
        return CreateRef<COpenGLVertexBuffer>(size);
    }

    Ref<IIndexBuffer> IIndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        // @todo rendererAPI
        return CreateRef<COpenGLIndexBuffer>(indices, count);
    }
}
