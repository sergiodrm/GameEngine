#pragma once
#include "Texture.h"
#include "VoltEngine/Core/Core.h"


namespace Volt
{
    class CTextureManager : public IResourceManager<ITexture>, public ISingleton<CTextureManager>
    {
    public:
        virtual void OnCreate() override;
        virtual void OnDestroy() override;
    protected:
        virtual SharedPtr<ITexture> CreateResource() override;
    };
}
