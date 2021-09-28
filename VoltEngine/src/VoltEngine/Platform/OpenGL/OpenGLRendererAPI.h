#pragma once
#include "VoltEngine/Core/Core.h"
#include "VoltEngine/Renderer/RendererAPI.h"

namespace Volt
{
    class COpenGLRendererAPI : public IRendererAPI
    {
    public:
        virtual void Init() override;
        virtual void Clear() override;
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void DrawIndexed(const Ref<IVertexArray>& vertexArray, uint32_t indexCount) override;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    };
}
