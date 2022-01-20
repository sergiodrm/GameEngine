#include "Texture.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/Texture/OpenGLTexture2D.h"

namespace Volt
{
    SharedPtr<ITexture> ITexture::Create()
    {
        switch (IRendererAPI::GetAPI())
        {
            case IRendererAPI::API::OpenGL: return CreateSharedPtr<COpenGLTexture2D>();
            default: VOLT_LOG(Error, "Unknown API!");
        }
        return nullptr;
    }
}
