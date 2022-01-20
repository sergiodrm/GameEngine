#include "AssetRegister.h"

namespace Volt
{
    void CAssetRegister::AddAsset(const std::string& filepath, const SharedPtr<IAsset>& asset)
    {
        if (asset && !HasAsset(filepath))
        {
            m_assets[filepath] = asset;
        }
    }

    void CAssetRegister::RemoveAsset(const std::string& filepath)
    {
        m_assets.erase(filepath);
    }

    SharedPtr<IAsset> CAssetRegister::GetAsset(const std::string& filepath) const
    {
        SharedPtr<IAsset> asset = nullptr;
        const auto assetIterator = m_assets.find(filepath);
        if (assetIterator != m_assets.end())
        {
            asset = assetIterator->second;
        }
        return asset;
    }

    bool CAssetRegister::HasAsset(const std::string& filepath) const
    {
        return m_assets.count(filepath) != 0;
    }
}
