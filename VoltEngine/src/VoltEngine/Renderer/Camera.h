#pragma once

#include "glm/glm.hpp"

namespace Volt
{
    class CCamera
    {
    public:
        enum class EProjectionType : uint8_t
        {
            Orthographic,
            Perspective
        };

    public:
        CCamera();
        virtual ~CCamera() = default;

        EProjectionType GetProjectionType() const { return m_projectionType; }
        void SetProjectionType(EProjectionType type);

        const glm::mat4& GetProjection() const { return m_projection; }

        void SetOrthographic(float size, float nearClip, float farClip);
        void SetPerspective(float verticalFOV, float nearClip, float farClip);
        void SetViewport(uint32_t width, uint32_t height);

        float GetOrthographicSize() const { return m_orthographicSize; }
        void SetOrthographicSize(float size);
        float GetOrthographicNearClip() const { return m_orthographicNearClip; }
        void SetOrthographicNearClip(float nearClip);
        float GetOrthographicFarClip() const { return m_orthographicFarClip; }
        void SetOrthographicFarClip(float farClip);

        float GetPerspectiveVerticalFOV() const { return m_perspectiveFOV; }
        void SetPerspectiveVerticalFOV(float fov);
        float GetPerspectiveNearClip() const { return m_perspectiveNearClip; }
        void SetPerspectiveNearClip(float nearClip);
        float GetPerspectiveFarClip() const { return m_perspectiveFarClip; }
        void SetPerspectiveFarClip(float farClip);

    protected:
        void RecalculateProjection();

    protected:
        EProjectionType m_projectionType {EProjectionType::Orthographic};
        glm::mat4 m_projection {1.f};

        float m_aspectRatio {16.f / 9.f};

        // Orthographic
        float m_orthographicSize {10.f};
        float m_orthographicNearClip {-1.f};
        float m_orthographicFarClip {1.f};

        // Perspective
        float m_perspectiveFOV {glm::radians(45.f)};
        float m_perspectiveNearClip {0.01f};
        float m_perspectiveFarClip {1000.f};
    };
}
