#pragma once
#include <future>
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

        void AsyncLoadFromFile(const std::string& filepath)
        {
            const std::future<void> future = std::async(std::launch::async, &IAssetLoader::Load, this, filepath, true);
        }

        void LoadFromFile(const std::string& filepath)
        {
            Load(filepath, false);
            LoadDataInAsset();
        }

        virtual void LoadDataInAsset() = 0;

        virtual const class IAsset* GetAsset() const { return m_asset; }
        virtual class IAsset* GetAsset() { return m_asset; }
        virtual void SetAsset(IAsset* asset) { m_asset = asset; }

    protected:
        virtual void Load(std::string filepath, bool async) = 0;

    private:
        class IAsset* m_asset;
    };
}
