#include "Entity.h"

namespace Volt
{
    CEntity::CEntity(CScene* scene)
        : m_sceneContext(scene)
    {
        VOLT_ASSERT(m_sceneContext, "Entity was created without scene context!");
    }

    CEntity::~CEntity()
    {
        for (CComponent* it : m_components)
        {
            delete it;
        }
        m_components.clear();
    }
}
