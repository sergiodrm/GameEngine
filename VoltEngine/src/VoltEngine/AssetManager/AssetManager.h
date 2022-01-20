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
        static SharedPtr<T> CreateEmptyAsset(const std::string& name);

        template <typename T>
        static SharedPtr<T> LoadAsset(const std::string& filepath);

        template <typename T>
        SharedPtr<T> CreateEmpty(const std::string& name);

        template <typename T>
        SharedPtr<T> Load(const std::string& filepath);

        /** Thread safe: should be called in the main thread at the beginning of the frame to process remaining loads. */
        void ProcessRequests();

        /** Thread safe: called from IAssetLoaders in background thread to push them into the requests queue. */
        void PushLoadRequest(IAssetLoader* assetLoader);

    private:
        std::queue<IAssetLoader*> m_loadRequests;
        std::vector<IAssetLoader*> m_assetLoaders;
        CAssetRegister m_assetRegister;
        std::mutex m_loadRequestsMutex;
    };

    template <typename T>
    SharedPtr<T> CAssetManager::CreateEmptyAsset(const std::string& name)
    {
        return Get().Load<T>(name);
    }

    template <typename T>
    SharedPtr<T> CAssetManager::LoadAsset(const std::string& filepath)
    {
        return Get().Load<T>(filepath);
    }

    template <typename T>
    SharedPtr<T> CAssetManager::CreateEmpty(const std::string& name)
    {
        T* asset = nullptr;
        if (T::GetStaticType().template IsA<IAsset>())
        {
            if (m_assetRegister.HasAsset(name))
            {
                asset = dynamic_cast<T*>(m_assetRegister.GetAsset(name));
            }
            else
            {
                asset = T::Create();
                m_assetRegister.AddAsset(name, asset);
            }
        }
        return asset;
    }

    template <typename T>
    SharedPtr<T> CAssetManager::Load(const std::string& filepath)
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
                asset = T::Create();
                m_assetRegister.AddAsset(filepath, asset);

                IAssetLoader* assetLoader = asset->CreateLoader();
                m_assetLoaders.push_back(assetLoader);
                assetLoader->StartAsyncLoad(filepath);
            }
        }
        return asset;
    }

    template <>
    inline SharedPtr<IAsset> CAssetManager::Load(const std::string& filepath)
    {
        VOLT_ASSERT(false, "CAssetManager::Load must received a specialization of IAsset!");
        return nullptr;
    }
}
