#include "Texture.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace Volt
{
    Ref<ITexture> ITexture::Create(uint32_t width, uint32_t height)
    {
        switch (IRendererAPI::GetAPI())
        {
            case IRendererAPI::API::OpenGL: return CreateRef<COpenGLTexture2D>(width, height);
            default: VOLT_LOG(Error, "Unknown API!");
        }
        return nullptr;
    }

    Ref<ITexture> ITexture::Create(const std::string& filepath)
    {
        switch (IRendererAPI::GetAPI())
        {
            case IRendererAPI::API::OpenGL: return CreateRef<COpenGLTexture2D>(filepath);
            default: VOLT_LOG(Error, "Unknown API!");
        }
        return nullptr;
    }
}
