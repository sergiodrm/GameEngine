#include "GraphicsContext.h"

#include "RendererAPI.h"
#include "VoltEngine/Core/Log.h"
#include "VoltEngine/Platform/OpenGL/OpenGLGraphicsContext.h"


namespace Volt
{
    Ref<IGraphicsContext> IGraphicsContext::Create()
    {
        switch (IRendererAPI::GetAPI())
        {
            case IRendererAPI::API::OpenGL: return CreateRef<COpenGLGraphicsContext>();
            default:
                VOLT_ASSERT(false, "Unknown renderer API");
        }
        return nullptr;
    }
}
