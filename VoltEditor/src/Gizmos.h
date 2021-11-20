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

        void DrawGridGizmo(const glm::mat4& transform, const glm::mat4& projection, const glm::mat4& view);
        void DrawEntityTransformGizmo(CEntity* entity, const glm::mat4& projection, const glm::mat4& view);
    private:
        CScene* m_sceneContext;

        int32_t m_gizmoOperationType {0};
    };
}
