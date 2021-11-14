#include "TextureManager.h"

#include "Texture.h"

namespace Volt
{
    void CTextureManager::OnCreate()
    {
        VOLT_LOG(Info, "Texture manager was created correctly.");
    }

    void CTextureManager::OnDestroy()
    {
        VOLT_LOG(Info, "Texture manager was destroyed correctly.");
    }

    SharedPtr<IResource> CTextureManager::CreateResourceImplementation(uint32_t resourceId, const std::string& filepath)
    {
        return ITexture::Create(this, filepath, resourceId);
    }
}