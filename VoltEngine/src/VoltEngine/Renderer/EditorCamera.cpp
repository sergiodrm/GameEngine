#include "EditorCamera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

#include "VoltEngine/Core/Input.h"
#include "VoltEngine/Core/MouseCodes.h"
#include "VoltEngine/Core/KeyCodes.h"
#include "VoltEngine/Events/Event.h"
#include "VoltEngine/Events/MouseEvent.h"

namespace Volt
{
    CEditorCamera::CEditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
        : CCamera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip)), m_fov(fov), m_aspectRatio(aspectRatio), m_nearClip(nearClip), m_farClip(farClip)
    {
        UpdateView();
    }

    void CEditorCamera::OnUpdate(float elapsedSeconds)
    {
        if (IInput::IsKeyPressed(KeyCodes::LeftShift))
        {
            const glm::vec2& mouse {IInput::GetMouseX(), IInput::GetMouseY()};
            const glm::vec2 delta = (mouse - m_initialMousePosition) * 0.003f;
            m_initialMousePosition = mouse;

            if (IInput::IsMouseButtonPressed(Mouse::ButtonMiddle))
                MousePan(delta);
            else if (IInput::IsMouseButtonPressed(Mouse::ButtonLeft))
                MouseRotate(delta);
            else if (IInput::IsMouseButtonPressed(Mouse::ButtonRight))
                MouseZoom(delta.y);
        }
        else
        {
            m_initialMousePosition = {IInput::GetMouseX(), IInput::GetMouseY()};
        }
        UpdateView();
    }

    void CEditorCamera::OnEvent(CEvent& e)
    {
        CEventDispatcher dispatcher(e);
        dispatcher.Dispatch<CMouseScrolledEvent>(BIND_FUNCTION(OnMouseScroll));
    }

    void CEditorCamera::SetViewportSize(float width, float height)
    {
        m_viewportWidth = width;
        m_viewportHeight = height;
        UpdateProjection();
    }

    glm::vec3 CEditorCamera::GetUpDirection() const { return rotate(GetOrientation(), glm::vec3(0.f, 1.f, 0.f)); }
    glm::vec3 CEditorCamera::GetRightDirection() const { return rotate(GetOrientation(), glm::vec3(1.f, 0.f, 0.f)); }
    glm::vec3 CEditorCamera::GetForwardDirection() const { return rotate(GetOrientation(), glm::vec3(0.f, 0.f, -1.f)); }
    glm::quat CEditorCamera::GetOrientation() const { return glm::quat({-m_pitch, -m_yaw, 0.f}); }

    void CEditorCamera::UpdateProjection()
    {
        m_aspectRatio = m_viewportWidth / m_viewportHeight;
        m_projection = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearClip, m_farClip);
    }

    void CEditorCamera::UpdateView()
    {
        m_position = CalculatePosition();
        const glm::quat orientation = GetOrientation();
        m_viewMatrix = translate(glm::mat4(1.f), m_position) * toMat4(orientation);
        m_viewMatrix = inverse(m_viewMatrix);
    }

    bool CEditorCamera::OnMouseScroll(CMouseScrolledEvent& e)
    {
        const float delta = e.GetDeltaY() * 0.1f;
        MouseZoom(delta);
        UpdateView();
        return false;
    }

    void CEditorCamera::MousePan(const glm::vec2& delta)
    {
        auto [xSpeed, ySpeed] = PanSpeed();
        m_focalPoint += -GetRightDirection() * delta.x * xSpeed * m_distance;
        m_focalPoint += GetUpDirection() * delta.y * ySpeed * m_distance;
    }

    void CEditorCamera::MouseRotate(const glm::vec2& delta)
    {
        const float yawSign = GetUpDirection().y < 0.f ? -1.f : 1.f;
        m_yaw += yawSign * delta.x * RotationSpeed();
        m_pitch += delta.y * RotationSpeed();
    }

    void CEditorCamera::MouseZoom(float delta)
    {
        m_distance -= delta * ZoomSpeed();
        if (m_distance < 1.f)
        {
            m_focalPoint += GetForwardDirection();
            m_distance = 1.f;
        }
    }

    glm::vec3 CEditorCamera::CalculatePosition() const { return m_focalPoint - GetForwardDirection() * m_distance; }

    std::pair<float, float> CEditorCamera::PanSpeed() const
    {
        const float x = std::min(m_viewportWidth * 0.001f, 2.4f);
        const float xFactor = 0.0366f * x * x - 0.1778f * x + 0.3021f;

        const float y = std::min(m_viewportHeight * 0.001f, 2.4f);
        const float yFactor = 0.0366f * y * y - 0.1778f * y + 0.3021f;
        return {xFactor, yFactor};
    }

    float CEditorCamera::RotationSpeed() const { return 0.8f; }

    float CEditorCamera::ZoomSpeed()
    {
        const float distance = std::max(m_distance * 0.2f, 0.f);
        const float speed = std::min(distance * distance, 100.f);
        return speed;
    }
}
