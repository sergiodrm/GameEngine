#include "GraphicsContext.h"

#include "RendererAPI.h"
#include "VoltEngine/Core/Log.h"
#include "Platform/OpenGL/OpenGLGraphicsContext.h"


namespace Volt
{
    SharedPtr<IGraphicsContext> IGraphicsContext::Create()
    {
        switch (IRendererAPI::GetAPI())
        {
            case IRendererAPI::API::OpenGL: return CreateSharedPtr<COpenGLGraphicsContext>();
            default:
                VOLT_ASSERT(false, "Unknown renderer API");
        }
        return nullptr;
    }
}
