#pragma once
#include <vector>

#include "VoltEngine/Core/Core.h"

namespace Volt
{
    class CEntity;

    class CEntitiesRegistry
    {
        using Registry = std::vector<Ref<CEntity>>;
        using RegistryIterator = Registry::iterator;
        using RegistryConstIterator = Registry::const_iterator;

    public:
        template <typename ... Args>
        Ref<CEntity> Create(Args&& ... args)
        {
            Ref<CEntity> entity = CreateRef<CEntity>(std::forward<Args>(args)...);
            m_registry.push_back(entity);
            return entity;
        }

        void Delete(const Ref<CEntity>& entity);

        const Registry& GetRegistry() const { return m_registry; }
        Registry& GetRegistry() { return m_registry; }

        //RegistryIterator begin() { return m_registry.begin(); }
        //RegistryIterator end() { return m_registry.end(); }
        //RegistryConstIterator begin() const { return m_registry.begin(); }
        //RegistryConstIterator end() const { return m_registry.end(); }

    private:
        Registry m_registry;
    };
}
