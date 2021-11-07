#include "Component.h"

#include "VoltEngine/Core/Log.h"

namespace Volt
{
    CComponent::CComponent(CEntity* owner)
        : m_owner(owner)
    {
        VOLT_ASSERT(m_owner, "Component was created with an empty entity!");
    }
}
