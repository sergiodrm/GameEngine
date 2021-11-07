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

    private:
        Registry m_registry;
    };
}
