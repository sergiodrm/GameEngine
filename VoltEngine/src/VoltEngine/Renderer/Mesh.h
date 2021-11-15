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
    };
}
