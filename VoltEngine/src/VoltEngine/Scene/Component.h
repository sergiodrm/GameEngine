#pragma once

#include <type_traits>

#include "VoltEngine/RTTI/TypeMacros.h"

namespace Volt
{
    class CEntity;

    class CComponent
    {
    DECLARE_BASE_CLASS(CComponent)
    public:
        template <typename T, typename ... Args>
        static T* Create(CEntity* owner, Args&& ... args)
        {
            T* component = new T(std::forward<Args>(args)...);
            component->m_owner = owner;
            return component;
        }

        virtual ~CComponent() = default;

        const CEntity* GetOwner() const { return m_owner; }
        CEntity* GetOwner() { return m_owner; }

    private:
        CEntity* m_owner {nullptr};
    };
}
