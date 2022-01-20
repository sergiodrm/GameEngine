#pragma once
#include <string>
#include <queue>
#include <filesystem>

#include "Asset.h"
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
        SharedPtr<T> asset = nullptr;
        if (T::GetStaticType() == IAsset::GetStaticType())
        {
            if (m_assetRegister.HasAsset(name))
            {
                asset = std::dynamic_pointer_cast<T>(m_assetRegister.GetAsset(name));
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
        SharedPtr<T> asset = nullptr;

        const std::filesystem::path absolutePath = std::filesystem::absolute(filepath);
        const std::filesystem::path normalizedPath = absolutePath.lexically_normal();
        const std::string normPathStr = normalizedPath.string();
        const std::string filename = normalizedPath.filename().string();
        VOLT_ASSERT(normPathStr.empty() | filename.empty(), "Invalid asset filepath");
        if (T::GetStaticType() == IAsset::GetStaticType())
        {
            if (m_assetRegister.HasAsset(normPathStr))
            {
                asset = std::dynamic_pointer_cast<T>(m_assetRegister.GetAsset(normPathStr));
            }
            else
            {
                asset = T::Create();
                asset->SetPath(normPathStr);
                m_assetRegister.AddAsset(normPathStr, asset);

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
