#pragma once
#include <vector>

#include "Component.h"
#include "VoltEngine/Core/Core.h"
#include "VoltEngine/Core/Log.h"

namespace Volt
{
    class CScene;
    class CComponent;

    class CEntity
    {
    public:
        CEntity(CScene* scene);
        ~CEntity();

        template <typename T, typename ... Args>
        T* AddComponent(Args&& ... args)
        {
            VOLT_ASSERT(!HasComponent<T>(), "Entity has already that component!");
            //Ref<T> component = CreateRef<T>(this, std::forward<Args>(args)...);
            T* component = new T(this, std::forward<Args>(args)...);
            m_components.push_back(component);
            return component;
        }

        template <typename T>
        const T* GetComponent() const
        {
            for (const CComponent* it : m_components)
            {
                if (it->GetDynamicType() == T::GetStaticType())
                {
                    return reinterpret_cast<T*>(it);
                }
            }
            return nullptr;
        }

        template <typename T>
        T* GetComponent()
        {
            for (CComponent* it : m_components)
            {
                if (it->GetDynamicType() == T::GetStaticType())
                {
                    return reinterpret_cast<T*>(it);
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

    private:
        CScene* m_sceneContext;
        std::vector<CComponent*> m_components;
    };
}
