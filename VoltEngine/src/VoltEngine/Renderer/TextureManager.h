#pragma once
#include "VoltEngine/Core/Core.h"
#include "VoltEngine/Resources/ResourceManager.h"


namespace Volt
{
    class ITexture;

    class CTextureManager : public IResourceManager, public ISingleton<CTextureManager>
    {
    public:
        virtual void OnCreate() override;
        virtual void OnDestroy() override;
    protected:
        virtual Ref<IResource> CreateResourceImplementation(uint32_t resourceId, const std::string& filepath) override;
    };
}
