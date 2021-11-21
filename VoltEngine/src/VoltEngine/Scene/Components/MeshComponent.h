#pragma once

#include "VoltEngine/Core/Core.h"
#include "VoltEngine/Scene/Component.h"

namespace Volt
{
    class IMesh;

    class CMeshComponent : public CComponent
    {
    DECLARE_DERIVED_CLASS(CMeshComponent, CComponent);
    public:
        CMeshComponent(const SharedPtr<IMesh>& mesh = nullptr)
            : m_mesh(mesh) {}

        const SharedPtr<IMesh>& GetMesh() const { return m_mesh; }
        void SetMesh(const SharedPtr<IMesh>& mesh) { m_mesh = mesh; }
    private:
        SharedPtr<IMesh> m_mesh;
    };
}
