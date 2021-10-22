#pragma once
#include "glm/glm.hpp"


namespace Volt
{
    struct SRenderer2DStats
    {
        uint32_t DrawCalls;
        uint32_t QuadCount;

        uint32_t GetVerticesCount() const { return QuadCount * 4; }
        uint32_t GetIndicesCount() const { return QuadCount * 6; }
        uint32_t GetTrianglesCount() const { return QuadCount * 2; }
    };

    class CRenderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const class CCamera& camera, const glm::mat4& transform);
        static void EndScene();
        static const SRenderer2DStats& GetStats();

        static void DrawQuad(const glm::vec2& position, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec4& color);

        static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color);

        static void DrawQuad(const glm::vec2& position, const glm::vec3& rotation, const glm::vec2& scale, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec3& rotation, const glm::vec2& scale, const glm::vec4& color);

        static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
    protected:
        static void Flush();
        static void Reset();
        static void UpdateBatch();
    };
}
