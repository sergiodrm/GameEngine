#pragma once
#include <string>

#include "VoltEngine/Core/Core.h"
#include "VoltEngine/Resources/Resource.h"

namespace Volt
{
    class IMaterial;
    class IVertexArray;
    struct SVertexData;

    class IMesh : public IResource
    {
    public:
        static SharedPtr<IMesh> Create();

        IMesh() = default;
        virtual ~IMesh() override = default;

        virtual const SharedPtr<IVertexArray>& GetVertexArray() const = 0;
        virtual const std::vector<SVertexData>& GetVertexData() const = 0;
        virtual const std::vector<uint32_t>& GetIndexData() const = 0;
        virtual uint32_t GetNumTriangles() const = 0;
        virtual const SharedPtr<IMaterial>& GetMaterial() const = 0;
    };
}
