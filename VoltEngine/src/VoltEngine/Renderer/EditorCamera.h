#pragma once
#include <utility>

#include "Camera.h"

namespace Volt
{
    class CMouseScrolledEvent;
    class CEvent;
    class CTimestep;

    class CEditorCamera : public CCamera
    {
    public:
        CEditorCamera() = default;
        CEditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

        void OnUpdate(float elapsedSeconds);
        void OnEvent(CEvent& e);

        float GetDistance() const { return m_distance; }
        void SetDistance(float distance) { m_distance = distance; }

        void SetViewportSize(float width, float height);

        const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
        glm::mat4 GetViewProjection() const { return m_projection * m_viewMatrix; }

        glm::vec3 GetUpDirection() const;
        glm::vec3 GetRightDirection() const;
        glm::vec3 GetForwardDirection() const;

        const glm::vec3& GetPosition() const { return m_position; }
        glm::quat GetOrientation() const;

        float GetPitch() const { return m_pitch; }
        float GetYaw() const { return m_yaw; }

    private:
        void UpdateProjection();
        void UpdateView();

        bool OnMouseScroll(CMouseScrolledEvent& e);

        void MousePan(const glm::vec2& delta);
        void MouseRotate(const glm::vec2& delta);
        void MouseZoom(float delta);

        glm::vec3 CalculatePosition() const;

        std::pair<float, float> PanSpeed() const;
        float RotationSpeed() const;
        float ZoomSpeed();

    private:
        float m_distance {10.f};

        float m_fov {45.f};
        float m_aspectRatio {1.778f};
        float m_nearClip {0.1f};
        float m_farClip {1000.f};

        float m_pitch {0.f};
        float m_yaw {0.f};

        glm::vec3 m_position {0.f};
        glm::mat4 m_viewMatrix {1.f};
        glm::vec3 m_focalPoint {0.f};

        glm::vec2 m_initialMousePosition {0.f};

        float m_viewportWidth {1280.f};
        float m_viewportHeight {720.f};
    };
}
