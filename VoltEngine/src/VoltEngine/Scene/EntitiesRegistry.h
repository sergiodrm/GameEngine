#pragma once
#include <vector>

#include "VoltEngine/Core/Core.h"
#include "Entity.h"
#include "Components/TagComponent.h"

namespace Volt
{
    class CTransformComponent;
    class CTagComponent;

    class CEntitiesRegistry
    {
        using Registry = std::vector<CEntity*>;
        using RegistryIterator = Registry::iterator;
        using RegistryConstIterator = Registry::const_iterator;

        using RegistryReverseIterator = Registry::reverse_iterator;
        using RegistryConstReverseIterator = Registry::const_reverse_iterator;

    public:
        ~CEntitiesRegistry();

        CEntity* Create(CScene* sceneContext, const std::string& name = {});
        void DestroyEntity(CEntity* entity);

        void Each(std::function<void(CEntity*)> callback);

        uint32_t GetSize() const { return static_cast<uint32_t>(m_registry.size()); }

        CEntity* operator[](uint32_t index);
        const CEntity* operator[](uint32_t index) const;

        RegistryIterator begin() { return m_registry.begin(); }
        RegistryIterator end() { return m_registry.end(); }
        RegistryConstIterator begin() const { return m_registry.begin(); }
        RegistryConstIterator end() const { return m_registry.end(); }

        struct SIterator
        {
            SIterator(CEntitiesRegistry& instance, bool iterateDestroyed = false);

            bool IsValid() const;

            operator bool() const;
            void operator++();
            CEntity* operator*() const;
        private:
            CEntitiesRegistry& m_instance;
            RegistryIterator m_iterator;
            bool m_iterateDestroyed;
        };

    protected:
        CEntity* FindFreeEntity() const;

    private:
        Registry m_registry;
        uint32_t m_idCounter {0};
    };
}
