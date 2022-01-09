#pragma once

#include "VoltEngine/RTTI/TypeMacros.h"

namespace Volt
{
    class IAsset
    {
    DECLARE_BASE_CLASS(IAsset);
    public:
        virtual ~IAsset() = default;
        virtual class IAssetLoader* CreateLoader() = 0;
        virtual bool IsLoaded() const = 0;
    };
}
