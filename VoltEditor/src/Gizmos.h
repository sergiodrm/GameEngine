#pragma once
#include <cstdint>
#include <glm/glm.hpp>

namespace Volt
{
    class CKeyPressedEvent;
    class CMouseButtonPressedEvent;
    class CEvent;
    class CEntity;
    class CScene;

    class CGizmo
    {
    public:
        CGizmo(CScene* sceneContext);

        void OnEvent(CEvent& e);
        void DrawGridGizmo(const glm::mat4& transform, const glm::mat4& projection, const glm::mat4& view);
        void DrawEntityTransformGizmo(CEntity* entity, const glm::mat4& projection, const glm::mat4& view);
    protected:
        bool OnKeyPressed(CKeyPressedEvent& e);
    private:
        CScene* m_sceneContext;

        int32_t m_gizmoOperationType {0};
    };
}
