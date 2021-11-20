#include "Mesh.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLMesh.h"
#include "VoltEngine/Core/Log.h"

namespace Volt
{
    SharedPtr<IMesh> IMesh::Create()
    {
        switch (IRendererAPI::GetAPI())
        {
            case IRendererAPI::API::OpenGL: return CreateSharedPtr<COpenGLMesh>();
            default: VOLT_ASSERT(false, "Invalid Renderer API.");
        }
        return nullptr;
    }
}
