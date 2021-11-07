#include "Camera.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "VoltEngine/Core/Log.h"

namespace Volt
{
    CCamera::CCamera()
    {
        RecalculateProjection();
    }

    void CCamera::SetProjectionType(EProjectionType type)
    {
        m_projectionType = type;
        RecalculateProjection();
    }

    void CCamera::SetOrthographic(float size, float nearClip, float farClip)
    {
        m_orthographicSize = size;
        m_orthographicNearClip = nearClip;
        m_orthographicFarClip = farClip;
        RecalculateProjection();
    }

    void CCamera::SetPerspective(float verticalFOV, float nearClip, float farClip)
    {
        m_perspectiveFOV = verticalFOV;
        m_perspectiveNearClip = nearClip;
        m_perspectiveFarClip = farClip;
        RecalculateProjection();
    }

    void CCamera::SetViewportSize(uint32_t width, uint32_t height)
    {
        m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        RecalculateProjection();
    }

    void CCamera::SetOrthographicSize(float size)
    {
        m_orthographicSize = size;
        RecalculateProjection();
    }

    void CCamera::SetOrthographicNearClip(float nearClip)
    {
        m_orthographicNearClip = nearClip;
        RecalculateProjection();
    }

    void CCamera::SetOrthographicFarClip(float farClip)
    {
        m_orthographicFarClip = farClip;
        RecalculateProjection();
    }

    void CCamera::SetPerspectiveVerticalFOV(float fov)
    {
        m_perspectiveFOV = fov;
        RecalculateProjection();
    }

    void CCamera::SetPerspectiveNearClip(float nearClip)
    {
        m_perspectiveNearClip = nearClip;
        RecalculateProjection();
    }

    void CCamera::SetPerspectiveFarClip(float farClip)
    {
        m_perspectiveFarClip = farClip;
        RecalculateProjection();
    }

    void CCamera::RecalculateProjection()
    {
        switch (m_projectionType)
        {
            case EProjectionType::Orthographic:
                {
                    const float left = -m_orthographicSize * 0.5f * m_aspectRatio;
                    const float right = m_orthographicSize * 0.5f * m_aspectRatio;
                    const float top = m_orthographicSize * 0.5f;
                    const float bottom = -m_orthographicSize * 0.5f;
                    m_projection = glm::ortho(left, right, bottom, top, m_orthographicNearClip, m_orthographicFarClip);
                }
                break;
            case EProjectionType::Perspective:
                {
                    m_projection = glm::perspective(m_perspectiveFOV, m_aspectRatio, m_perspectiveNearClip, m_perspectiveFarClip);
                }
                break;
            default:
                VOLT_LOG(Error, "Camera projection only supports orthographic and perspective!");
        }
    }
}
