#pragma once
#include "glm/vec4.hpp"
#include "VoltEngine/VoltEngine.h"

namespace Volt
{
    class CRenderCommand
    {
    public:
        static void Init();
        static void Shutdown();

        static void Clear();
        static void SetClearColor(const glm::vec4& color);

        static void DrawIndexed(const Ref<class IVertexArray>& vertexArray, uint32_t indexCount = 0);

    private:
        static class IRendererAPI* s_rendererAPI;
    };
}
