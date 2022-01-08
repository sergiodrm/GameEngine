#pragma once


namespace Volt
{
    class IAsset
    {
    public:
        virtual class IAssetLoader* CreateLoader() = 0;
        virtual bool IsLoaded() const = 0;
    };
}
