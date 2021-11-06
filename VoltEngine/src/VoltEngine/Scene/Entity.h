#pragma once
#include <vector>

#include "Component.h"
#include "VoltEngine/Core/Core.h"
#include "VoltEngine/Core/Log.h"

namespace Volt
{
    class CComponent;

    class CEntity
    {
    public:
        CEntity();

        template <typename T, typename ... Args>
        Ref<T> AddComponent(Args&& ... args)
        {
            VOLT_ASSERT(!HasComponent<T>(), "Entity has already that component!");
            Ref<T> component = CreateRef<T>(std::forward<Args>(args)...);
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
                    return it;
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
                    return it;
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

    private:
        std::vector<Ref<CComponent>> m_components;
    };
}
