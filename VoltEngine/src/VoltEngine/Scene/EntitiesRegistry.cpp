#include "EntitiesRegistry.h"

#include "Components/TransformComponent.h"

namespace Volt
{
    CEntitiesRegistry::~CEntitiesRegistry()
    {
        for (CEntity* it : m_registry)
        {
            delete it;
        }
        m_registry.clear();
    }

    CEntity* CEntitiesRegistry::Create(CScene* sceneContext, const std::string& name)
    {
        CEntity* entity = FindFreeEntity();
        if (!entity)
        {
            entity = new CEntity(sceneContext);
            entity->m_id = m_idCounter;
            m_registry.push_back(entity);
            ++m_idCounter;
        }
        else
        {
            entity->SetStatus(EEntityStatus::Active);
        }
#ifdef VOLT_DEBUG
        VOLT_LOG(Trace, "Entity created: [ID: {0}, Tag: {1}]", entity->GetID(), name.c_str());
#endif
        entity->AddComponent<CTagComponent>(name);
        entity->AddComponent<CTransformComponent>();
        return entity;
    }

    void CEntitiesRegistry::DestroyEntity(CEntity* entity)
    {
        RegistryIterator it = std::find(m_registry.begin(), m_registry.end(), entity);
        if (it != m_registry.end())
        {
            (*it)->Destroy();
        }
    }

    void CEntitiesRegistry::Each(std::function<void(CEntity*)> callback)
    {
        if (callback)
        {
            for (CEntity* it : m_registry)
            {
                if (it && it->GetStatus() == EEntityStatus::Active)
                {
                    callback(it);
                }
            }
        }
    }

    CEntity* CEntitiesRegistry::operator[](uint32_t index)
    {
        if (index < m_registry.size())
        {
            return m_registry[index];
        }
        return nullptr;
    }

    const CEntity* CEntitiesRegistry::operator[](uint32_t index) const
    {
        if (index < m_registry.size())
        {
            return m_registry[index];
        }
        return nullptr;
    }

    CEntitiesRegistry::SIterator::SIterator(CEntitiesRegistry& instance, bool iterateDestroyed)
        : m_instance(instance), m_iterator(instance.begin()), m_iterateDestroyed(iterateDestroyed) {}

    bool CEntitiesRegistry::SIterator::IsValid() const { return m_iterator != m_instance.end(); }

    CEntitiesRegistry::SIterator::operator bool() const
    {
        return IsValid();
    }

    void CEntitiesRegistry::SIterator::operator++()
    {
        ++m_iterator;
        if (!m_iterateDestroyed)
        {
            while (IsValid() && (*m_iterator)->GetStatus() == EEntityStatus::Destroyed)
            {
                ++m_iterator;
            }
        }
    }

    CEntity* CEntitiesRegistry::SIterator::operator*() const { return *m_iterator; }

    CEntity* CEntitiesRegistry::FindFreeEntity() const
    {
        for (CEntity* it : m_registry)
        {
            if (it && it->GetStatus() == EEntityStatus::Destroyed)
            {
                return it;
            }
        }
        return nullptr;
    }
}
