#include "Texture.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace Volt
{
    SharedPtr<ITexture> ITexture::Create(IResourceManager* creator, const std::string& name, uint32_t id)
    {
        switch (IRendererAPI::GetAPI())
        {
            case IRendererAPI::API::OpenGL: return CreateSharedPtr<COpenGLTexture2D>(creator, name, id);
            default: VOLT_LOG(Error, "Unknown API!");
        }
        return nullptr;
    }
}
