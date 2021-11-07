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

        template <typename T, typename ... Args>
        Ref<T> AddComponent(Args&& ... args)
        {
            VOLT_ASSERT(!HasComponent<T>(), "Entity has already that component!");
            Ref<T> component = CreateRef<T>(this, std::forward<Args>(args)...);
            m_components.push_back(component);
            return component;
        }

        template <typename T>
        const Ref<T>& GetComponent() const
        {
            for (const Ref<CComponent>& it : m_components)
            {
                if (it->GetStaticType() == T::GetStaticType())
                {
                    return Cast<T>(it);
                }
            }
            return nullptr;
        }

        template <typename T>
        Ref<T> GetComponent()
        {
            for (Ref<CComponent>& it : m_components)
            {
                if (it->GetStaticType() == T::GetStaticType())
                {
                    return Cast<T>(it);
                }
            }
            return nullptr;
        }

        template <typename T>
        bool HasComponent() const
        {
            for (const Ref<CComponent>& it : m_components)
            {
                if (it->GetStaticType() == T::GetStaticType())
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
        std::vector<Ref<CComponent>> m_components;
    };
}
