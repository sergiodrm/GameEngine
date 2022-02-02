#include "AssetManager.h"

namespace Volt
{
    void CAssetManager::ProcessRequests()
    {
        while (!m_loadRequests.empty())
        {
            m_loadRequestsMutex.lock();
            IAssetLoader* assetLoader = m_loadRequests.front();
            m_loadRequests.pop();
            m_loadRequestsMutex.unlock();
            if (assetLoader)
            {
                assetLoader->LoadDataInAsset();
                RemoveLoader(assetLoader);
            }
        }
    }

    void CAssetManager::PushLoadRequest(IAssetLoader* assetLoader)
    {
        if (assetLoader)
        {
            m_loadRequestsMutex.lock();
            m_loadRequests.push(assetLoader);
            m_loadRequestsMutex.unlock();
        }
    }

    void CAssetManager::RemoveLoader(IAssetLoader* loader)
    {
        const std::vector<UniquePtr<IAssetLoader>>::const_iterator it =
            std::find_if(m_assetLoaders.begin(), m_assetLoaders.end(),
                         [&](const UniquePtr<IAssetLoader>& other)
                         {
                             return loader == other.get();
                         });
        if (it != m_assetLoaders.end())
        {
            m_assetLoaders.erase(it);
        }
    }
}
