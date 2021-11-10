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
            m_registry.push_back(entity);
            entity->m_id = m_idCounter;
            entity->AddComponent<CTagComponent>(name);
            entity->AddComponent<CTransformComponent>();
            ++m_idCounter;
        }
        else
        {
            entity->SetStatus(EEntityStatus::Active);
            entity->GetComponent<CTagComponent>()->SetTag(name);
            entity->GetComponent<CTransformComponent>()->SetPosition(glm::vec3(0.f));
            entity->GetComponent<CTransformComponent>()->SetRotation(glm::vec3(0.f));
            entity->GetComponent<CTransformComponent>()->SetScale(glm::vec3(1.f));
        }
        return entity;
    }

    void CEntitiesRegistry::Delete(CEntity* entity)
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
