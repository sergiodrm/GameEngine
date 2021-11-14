#pragma once
#include <cstdint>
#include "glm/glm.hpp"
#include "VoltEngine/Core/Core.h"

namespace Volt
{
    class CRenderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void OnWindowResize(uint32_t width, uint32_t height);

        static void BeginScene(const SharedPtr<class CCamera>& camera);
        static void EndScene();
        static void Submit(const SharedPtr<class IShader>& shader, const SharedPtr<class IVertexArray>& vertexArray, uint32_t indexCount = 0);
    protected:
        struct SSceneData
        {
            glm::mat4 SceneProjection;
        };

        static SSceneData s_sceneData;
    };
}
