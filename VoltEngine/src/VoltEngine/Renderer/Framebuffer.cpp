#include "Framebuffer.h"

#include "RendererAPI.h"
#include "VoltEngine/Core/Log.h"
#include "VoltEngine/Platform/OpenGL/OpenGLFramebuffer.h"

namespace Volt
{
    Ref<IFramebuffer> IFramebuffer::Create(const SFramebufferSpecification& spec)
    {
        switch (IRendererAPI::GetAPI())
        {
            case IRendererAPI::API::OpenGL: return CreateRef<COpenGLFramebuffer>(spec);
        }
        VOLT_ASSERT(false, "Unknown renderer API!");
    }
}
