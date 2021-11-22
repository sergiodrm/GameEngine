#pragma once

#include <glm/glm.hpp>

#include "VoltEngine/Core/Core.h"

namespace Volt
{
    struct SLight;
    class IMesh;
    class IVertexArray;
    class CCamera;

    struct SRenderStats
    {
        uint32_t VertexCount;
        uint32_t TriangleCount;
        uint32_t DrawCallCount;
        uint32_t BatchSize;

        uint32_t GetIndexCount() const;
        uint32_t GetVertexUsedMemory() const;
        uint32_t GetIndexUsedMemory() const;

        void Reset();
    };

    class CRenderer3D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const CCamera& camera, const glm::mat4& transform);
        static void BeginScene(const glm::mat4& projection, const glm::mat4& view, const std::vector<SLight>& lights = {});
        static void EndScene();

        static void DrawMesh(const glm::mat4& transform, const SharedPtr<IMesh>& mesh);

        static const SRenderStats& GetStats() { return Stats; }

        static glm::vec3 AmbientColor;
        static float AmbientStrength;

    protected:
        static void UpdateBatch();
        static void Flush();

    private:
        static SRenderStats Stats;
    };
}
