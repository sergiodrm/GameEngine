#include "VertexArray.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Volt
{
    Ref<IVertexArray> IVertexArray::Create()
    {
        return CreateRef<COpenGLVertexArray>();
    }
}
