#pragma once
#include "VoltEngine/Renderer/Camera.h"
#include "VoltEngine/Scene/Component.h"

namespace Volt
{
    class CCameraComponent : public CComponent
    {
    DECLARE_DERIVED_CLASS(CCameraComponent, CComponent)
    public:
        CCameraComponent(bool primary = false)
            : m_primary(primary) {}

        CCamera& GetCamera() { return m_camera; }
        const CCamera& GetCamera() const { return m_camera; }

        bool IsPrimary() const { return m_primary; }
        void SetPrimary(bool primary) { m_primary = primary; }

    private:
        CCamera m_camera;
        bool m_primary {true};
    };
}
