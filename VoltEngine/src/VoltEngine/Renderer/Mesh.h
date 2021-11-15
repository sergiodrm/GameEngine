#pragma once
#include <string>

#include "VoltEngine/Core/Core.h"

namespace Volt
{
    class IVertexArray;
    struct SVertexData;

    class IMesh
    {
    public:
        static SharedPtr<IMesh> Create(const std::string& filepath);
        static SharedPtr<IMesh> Create(const std::vector<SVertexData>& vertices, const std::vector<uint32_t>& indices);

        IMesh() = default;
        virtual ~IMesh() = default;

        virtual const SharedPtr<IVertexArray>& GetVertexArray() const = 0;
        virtual const std::vector<SVertexData>& GetVertexData() const = 0;
        virtual const std::vector<uint32_t>& GetIndexData() const = 0;
        virtual uint32_t GetNumTriangles() const = 0;
    };
}
