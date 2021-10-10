#pragma once
#include "glm/glm.hpp"


namespace Volt
{
    class CRenderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const class CCamera& camera, const glm::mat4& transform);
        static void EndScene();

        static void DrawQuad(const glm::vec2& position, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec4& color);

        static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
    protected:
        static void Flush();
        static void Reset();
        static void UpdateBatch();
    };
}
