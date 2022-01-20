#pragma once
#include <unordered_map>

#include "VoltEngine/Core/Core.h"


namespace Volt
{
    class IAsset;

    class CAssetRegister
    {
    public:
        void AddAsset(const std::string& filepath, const SharedPtr<IAsset>& asset);
        void RemoveAsset(const std::string& filepath);
        SharedPtr<IAsset> GetAsset(const std::string& filepath) const;
        bool HasAsset(const std::string& filepath) const;

    private:
        std::unordered_map<std::string, SharedPtr<IAsset>> m_assets;
    };
}
