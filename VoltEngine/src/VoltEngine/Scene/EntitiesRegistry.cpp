#include "EntitiesRegistry.h"

namespace Volt
{
    void CEntitiesRegistry::Delete(const Ref<CEntity>& entity)
    {
        const RegistryConstIterator it = std::find(m_registry.begin(), m_registry.end(), entity);
        if (it != m_registry.end())
        {
            m_registry.erase(it);
        }
    }
}
