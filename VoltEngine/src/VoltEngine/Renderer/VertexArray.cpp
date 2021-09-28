#include "VertexArray.h"

#include "VoltEngine/Platform/OpenGL/OpenGLVertexArray.h"

namespace Volt
{
    Ref<IVertexArray> IVertexArray::Create()
    {
        return CreateRef<COpenGLVertexArray>();
    }
}
