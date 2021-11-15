#include "Mesh.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLMesh.h"
#include "VoltEngine/Core/Log.h"

namespace Volt
{
    SharedPtr<IMesh> IMesh::Create(const std::string& filepath)
    {
        switch (IRendererAPI::GetAPI())
        {
            case IRendererAPI::API::OpenGL: return CreateSharedPtr<COpenGLMesh>(filepath);
            default: VOLT_ASSERT(false, "Invalid Renderer API.");
        }
        return nullptr;
    }

    SharedPtr<IMesh> IMesh::Create(const std::vector<SVertexData>& vertices, const std::vector<uint32_t>& indices)
    {
        switch (IRendererAPI::GetAPI())
        {
            case IRendererAPI::API::OpenGL: return CreateSharedPtr<COpenGLMesh>(vertices, indices);
            default: VOLT_ASSERT(false, "Invalid Renderer API.");
        }
        return nullptr;
    }
}
