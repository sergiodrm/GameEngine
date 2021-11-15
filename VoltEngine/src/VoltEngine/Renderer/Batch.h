#pragma once
#include <cstdint>

#include "VoltEngine/Core/Core.h"

namespace Volt
{
    struct SVertexData;

    class IBatch
    {
    public:
        static SharedPtr<IBatch> Create(uint32_t maxNumTriangles);

        virtual ~IBatch() = default;

        virtual void AddTriangles(const std::vector<SVertexData>& vertices, const std::vector<uint32_t>& indices) = 0;

        virtual bool CanAddTriangles(uint32_t numTriangles) const = 0;
        virtual bool IsEmpty() const = 0;

        virtual void Render() const = 0;

        virtual void Clear() = 0;
    };
}
