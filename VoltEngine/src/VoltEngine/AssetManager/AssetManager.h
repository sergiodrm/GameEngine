#pragma once
#include <string>
#include <queue>

#include "AssetRegister.h"
#include "VoltEngine/Core/Log.h"
#include "AssetLoader.h"
#include "VoltEngine/Core/Singleton.h"

namespace Volt
{
    class CAssetManager : public ISingleton<CAssetManager>
    {
    public:
        template <typename T>
        T* Load(const std::string& filepath);

        void ProcessRequests();
    private:
        std::queue<IAssetLoader*> m_loadRequests;
        std::vector<IAssetLoader*> m_assetLoaders;
        CAssetRegister m_assetRegister;
    };

    template <typename T>
    T* CAssetManager::Load(const std::string& filepath)
    {
        T* asset = nullptr;
        if (T::GetStaticType().template IsA<IAsset>())
        {
            if (m_assetRegister.HasAsset(filepath))
            {
                asset = dynamic_cast<T*>(m_assetRegister.GetAsset(filepath));
            }
            else
            {
                asset = new T();
                m_assetRegister.AddAsset(filepath, asset);

                IAssetLoader* assetLoader = asset->CreateLoader();
                m_assetLoaders.push_back(assetLoader);
                assetLoader->StartAsyncLoad(filepath);
            }
        }
        return asset;
    }

    template <>
    IAsset* CAssetManager::Load(const std::string& filepath)
    {
        VOLT_ASSERT(false, "CAssetManager::Load must received a specialization of IAsset!");
        return nullptr;
    }
}
