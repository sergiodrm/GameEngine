#include "VertexArray.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Volt
{
    SharedPtr<IVertexArray> IVertexArray::Create()
    {
        return CreateSharedPtr<COpenGLVertexArray>();
    }
}
