#include "Gizmos.h"

#include <ImGuizmo.h>
#include <imgui.h>

#include "glm/gtc/type_ptr.hpp"
#include "VoltEngine/Core/Input.h"
#include "VoltEngine/Scene/Entity.h"
#include "VoltEngine/Scene/Components/TransformComponent.h"
#include "VoltEngine/Utils/Math.h"

namespace Volt
{
    CGizmo::CGizmo(CScene* sceneContext)
        : m_sceneContext(sceneContext), m_entityContext(nullptr), m_gizmoOperationType(ImGuizmo::OPERATION::TRANSLATE)
    { }

    void CGizmo::SetEntityContext(CEntity* entity)
    {
        m_entityContext = entity;
    }

    void CGizmo::OnUIRender(const glm::mat4& projection, const glm::mat4& view)
    {
        if (m_entityContext)
        {
            if (IInput::IsKeyPressed(KeyCodes::W))
                m_gizmoOperationType = ImGuizmo::OPERATION::TRANSLATE;
            if (IInput::IsKeyPressed(KeyCodes::E))
                m_gizmoOperationType = ImGuizmo::OPERATION::ROTATE;
            if (IInput::IsKeyPressed(KeyCodes::R))
                m_gizmoOperationType = ImGuizmo::OPERATION::SCALE;


            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            const float windowWidth = static_cast<float>(ImGui::GetWindowWidth());
            const float windowHeight = static_cast<float>(ImGui::GetWindowHeight());
            ImGuizmo::SetRect(ImGui::GetWindowPos().x,
                              ImGui::GetWindowPos().y,
                              windowWidth,
                              windowHeight);

            CTransformComponent* transformComponent = m_entityContext->GetComponent<CTransformComponent>();
            glm::mat4 transform = transformComponent->GetTransform();

            Manipulate(
                       value_ptr(view),
                       value_ptr(projection),
                       static_cast<ImGuizmo::OPERATION>(m_gizmoOperationType),
                       ImGuizmo::MODE::LOCAL,
                       value_ptr(transform)
                      );

            if (ImGuizmo::IsUsing())
            {
                glm::vec3 position;
                glm::vec3 orientation;
                glm::vec3 scale;
                CMath::DescomposeTransform(transform, position, orientation, scale);

                const glm::vec3 rotation = orientation - transformComponent->GetRotation();
                transformComponent->SetPosition(position);
                transformComponent->SetRotation(transformComponent->GetRotation() + rotation);
                transformComponent->SetScale(scale);
            }
        }
    }
}
