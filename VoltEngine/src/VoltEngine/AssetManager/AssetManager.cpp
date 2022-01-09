#include "AssetManager.h"

namespace Volt
{
    void CAssetManager::ProcessRequests()
    {
        m_loadRequestsMutex.lock();
        while (!m_loadRequests.empty())
        {
            IAssetLoader* assetLoader = m_loadRequests.front();
            m_loadRequests.pop();
            if (assetLoader)
            {
                assetLoader->LoadDataInAsset();

                std::vector<IAssetLoader*>::iterator it = std::find(m_assetLoaders.begin(), m_assetLoaders.end(), assetLoader);
                if (it != m_assetLoaders.end())
                {
                    m_assetLoaders.erase(it);
                }
                delete assetLoader;
            }
        }
        m_loadRequestsMutex.unlock();
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
}
