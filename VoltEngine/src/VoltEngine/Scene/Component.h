#pragma once

#include "VoltEngine/RTTI/TypeMacros.h"

namespace Volt
{
    class CEntity;

    class CComponent
    {
    DECLARE_BASE_CLASS()
    public:
        CComponent(CEntity* owner);

        const CEntity* GetOwner() const { return m_owner; }
        CEntity* GetOwner() { return m_owner; }

    private:
        CEntity* m_owner;
    };
}
