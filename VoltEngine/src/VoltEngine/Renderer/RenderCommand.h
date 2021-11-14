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

        static void DrawIndexed(const SharedPtr<class IVertexArray>& vertexArray, uint32_t indexCount = 0);

        static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

    private:
        static class IRendererAPI* s_rendererAPI;
    };
}
