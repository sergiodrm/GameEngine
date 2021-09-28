#pragma once
#include "Core.h"

#include "glm/vec4.hpp"

namespace Volt
{
    class IRendererAPI
    {
    public:
        enum class API : uint8_t
        {
            None = 0,
            OpenGL
        };

        virtual void Init() = 0;
        virtual void Clear() = 0;
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void DrawIndexed(const Ref<class IVertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

        static API GetAPI() { return API::OpenGL; }
    };
}
