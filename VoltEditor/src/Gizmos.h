#pragma once
#include <cstdint>
#include <glm/glm.hpp>

namespace Volt
{
    class CEntity;
    class CScene;

    class CGizmo
    {
    public:
        CGizmo(CScene* sceneContext);

        void SetEntityContext(CEntity* entity);
        void OnUIRender(const glm::mat4& projection, const glm::mat4& view);
    private:
        CScene* m_sceneContext;
        CEntity* m_entityContext;

        int32_t m_gizmoOperationType {0};
    };
}
