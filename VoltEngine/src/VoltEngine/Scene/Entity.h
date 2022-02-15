#pragma once
#include <vector>

#include "Component.h"
#include "VoltEngine/Core/Core.h"
#include "VoltEngine/Core/Log.h"

namespace Volt
{
    class CScene;
    class CComponent;

    enum class EEntityStatus : uint8_t
    {
        Destroyed,
        Active
    };

    class CEntity
    {
        friend class CEntitiesRegistry;
    public:
        enum : int32_t
        {
            INVALID_ENTITY_HANDLE = -1
        };

        CEntity(CScene* scene);
        ~CEntity();

        void Destroy();

        EEntityStatus GetStatus() const { return m_status; }
        void SetStatus(EEntityStatus status) { m_status = status; }
        uint32_t GetID() const { return m_id; }

        template <typename T, typename ... Args>
        T* AddComponent(Args&& ... args)
        {
            VOLT_ASSERT(!HasComponent<T>(), "Entity has already that component!");
            T* component = CComponent::Create<T, Args...>(this, std::forward<Args>(args)...);
            m_components.push_back(component);
            OnComponentAdded(component);
            return component;
        }

        template <typename T>
        void RemoveComponent()
        {
            for (std::vector<CComponent*>::iterator it(m_components.begin());
                 it != m_components.end();
                 ++it)
            {
                if ((*it)->IsA<T>())
                {
                    OnComponentRemoved(*it);
                    delete (*it);
                    m_components.erase(it);
                    break;
                }
            }
        }

        template <typename T>
        const T* GetComponent() const
        {
            for (const CComponent* it : m_components)
            {
                if (it->IsA<T>())
                {
                    return static_cast<const T*>(it);
                }
            }
            return nullptr;
        }

        template <typename T>
        T* GetComponent()
        {
            for (CComponent* it : m_components)
            {
                if (it->IsA<T>())
                {
                    return static_cast<T*>(it);
                }
            }
            return nullptr;
        }

        template <typename T>
        bool HasComponent() const
        {
            for (CComponent* it : m_components)
            {
                if (it->GetDynamicType() == T::GetStaticType())
                {
                    return true;
                }
            }
            return false;
        }

        const CScene* GetSceneContext() const { return m_sceneContext; }
        CScene* GetSceneContext() { return m_sceneContext; }

        bool operator ==(const CEntity& other) const { return m_id == other.m_id; }

    protected:
        void OnComponentAdded(CComponent* component);

        void OnComponentRemoved(CComponent* component);

        void DestroyComponents();

    private:
        CScene* m_sceneContext;
        std::vector<CComponent*> m_components;
        uint32_t m_id;
        EEntityStatus m_status;
    };
}
