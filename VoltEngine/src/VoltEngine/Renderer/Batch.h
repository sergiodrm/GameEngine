#pragma once
#include <cstdint>
#include <glm/glm.hpp>

#include "VoltEngine/Core/Core.h"

namespace Volt
{
    struct SVertexData;

    struct SBatchConfig
    {
        glm::mat4 Transform {1.f};
        uint32_t TextureIndex {0};
    };

    class IBatch
    {
    public:
        static SharedPtr<IBatch> Create(uint32_t maxNumTriangles);

        virtual ~IBatch() = default;

        virtual void AddTriangles(
            const std::vector<SVertexData>& vertices,
            const std::vector<uint32_t>& indices,
            const SBatchConfig& batchConfig = {}) = 0;

        virtual bool CanAddTriangles(uint32_t numTriangles) const = 0;
        virtual bool IsEmpty() const = 0;

        virtual void Render() const = 0;

        virtual void Clear() = 0;
    };
}
