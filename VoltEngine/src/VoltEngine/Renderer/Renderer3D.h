#pragma once

#include <glm/glm.hpp>

#include "VoltEngine/Core/Core.h"

namespace Volt
{
    class IMesh;
    class IVertexArray;
    class CCamera;

    class CRenderer3D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const CCamera& camera, const glm::mat4& transform);
        static void EndScene();

        static void DrawMesh(const glm::mat4& transform, const SharedPtr<IMesh>& mesh);
    };
}
