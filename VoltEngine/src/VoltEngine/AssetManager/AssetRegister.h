#pragma once
#include <unordered_map>


namespace Volt
{
    class CAssetRegister
    {
    public:
        void AddAsset(const std::string& filepath, class IAsset* asset);
        void RemoveAsset(const std::string& filepath);
        class IAsset* GetAsset(const std::string& filepath) const;
        bool HasAsset(const std::string& filepath) const;

    private:
        std::unordered_map<std::string, class IAsset*> m_assets;
    };
}
