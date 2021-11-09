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

    Ref<CEntity> CEntitiesRegistry::operator[](uint32_t index)
    {
        if (index < m_registry.size())
        {
            return m_registry[index];
        }
        return nullptr;
    }

    Ref<const CEntity> CEntitiesRegistry::operator[](uint32_t index) const
    {
        if (index < m_registry.size())
        {
            return m_registry[index];
        }
        return nullptr;
    }
}
