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
        struct SLoadSpecs
        {
            enum ELoadType : uint8_t
            {
                Sync,
                Async
            };

            ELoadType LoadType {Sync};
        };

        template <typename T>
        static SharedPtr<T> CreateEmptyAsset(const std::string& name);

        template <typename T>
        static SharedPtr<T> LoadAsset(const std::string& filepath, const SLoadSpecs& loadSpecs = SLoadSpecs());

        template <typename T>
        SharedPtr<T> CreateEmpty(const std::string& name);

        template <typename T>
        SharedPtr<T> Load(const std::string& filepath, const SLoadSpecs& loadSpecs);

        /** Thread safe: should be called in the main thread at the beginning of the frame to process remaining loads. */
        void ProcessRequests();

        /** Thread safe: called from IAssetLoaders in background thread to push them into the requests queue. */
        void PushLoadRequest(IAssetLoader* assetLoader);

    protected:
        void RemoveLoader(IAssetLoader* loader);

    private:
        std::queue<IAssetLoader*> m_loadRequests;
        std::vector<UniquePtr<IAssetLoader>> m_assetLoaders;
        CAssetRegister m_assetRegister;
        std::mutex m_loadRequestsMutex;
    };

    template <typename T>
    SharedPtr<T> CAssetManager::CreateEmptyAsset(const std::string& name)
    {
        return Get().CreateEmpty<T>(name);
    }

    template <typename T>
    SharedPtr<T> CAssetManager::LoadAsset(const std::string& filepath, const SLoadSpecs& loadSpecs)
    {
        return Get().Load<T>(filepath, loadSpecs);
    }

    template <typename T>
    SharedPtr<T> CAssetManager::CreateEmpty(const std::string& name)
    {
        SharedPtr<T> asset = nullptr;
        if (T::IsA(IAsset::GetStaticType()))
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
    SharedPtr<T> CAssetManager::Load(const std::string& filepath, const SLoadSpecs& loadSpecs)
    {
        SharedPtr<T> asset = nullptr;

        const std::filesystem::path absolutePath = std::filesystem::absolute(filepath);
        const std::filesystem::path normalizedPath = absolutePath.lexically_normal();
        const std::string normPathStr = normalizedPath.string();
        const std::string filename = normalizedPath.filename().string();
        if (!normPathStr.empty() & !filename.empty() && T::IsA(IAsset::GetStaticType()))
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

                UniquePtr<IAssetLoader> assetLoader = asset->CreateLoader();
                switch (loadSpecs.LoadType)
                {
                    case SLoadSpecs::Sync:
                        {
                            assetLoader->LoadFromFile(filepath);
                        }
                        break;
                    case SLoadSpecs::Async:
                        {
                            IAssetLoader* assetLoaderPtr = assetLoader.get();
                            m_assetLoaders.push_back(std::move(assetLoader));
                            assetLoaderPtr->AsyncLoadFromFile(filepath);
                        }
                        break;
                    default:
                        VOLT_ASSERT(false, "Invalid LoadSpecs info.");
                }
            }
        }
        else
        {
            VOLT_LOG(Warning, "Couldn't find asset in {0} location.", filepath.c_str());
        }
        return asset;
    }

    template <>
    inline SharedPtr<IAsset> CAssetManager::Load(const std::string& filepath, const SLoadSpecs& loadSpecs)
    {
        VOLT_ASSERT(false, "CAssetManager::Load Invalid IAsset type.");
        return nullptr;
    }
}
