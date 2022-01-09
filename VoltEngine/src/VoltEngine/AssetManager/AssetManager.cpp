#include "AssetManager.h"

namespace Volt
{
    void CAssetManager::ProcessRequests()
    {
        while (!m_loadRequests.empty())
        {
            IAssetLoader* assetLoader = m_loadRequests.front();
            m_loadRequests.pop();
            if (assetLoader)
            {
                assetLoader->LoadAssetData();

                std::vector<IAssetLoader*>::iterator it = std::find(m_assetLoaders.begin(), m_assetLoaders.end(), assetLoader);
                if (it != m_assetLoaders.end())
                {
                    m_assetLoaders.erase(it);
                }
                delete assetLoader;
            }
        }
    }
}
