#pragma once
#include <string>
#include "VoltEngine/Core/Log.h"

namespace Volt
{
    class IAssetLoader
    {
    public:
        IAssetLoader(class IAsset* asset)
            : m_asset(asset)
        {
            VOLT_ASSERT(asset != nullptr, "Asset can't be nullptr!");
        }

        virtual ~IAssetLoader() = default;

        virtual void StartAsyncLoad(const std::string& filepath) = 0;
        virtual void LoadDataInAsset() = 0;

        virtual const class IAsset* GetAsset() const { return m_asset; }
        virtual class IAsset* GetAsset() { return m_asset; }
        virtual void SetAsset(IAsset* asset) { m_asset = asset; }
    private:
        class IAsset* m_asset;
    };
}
