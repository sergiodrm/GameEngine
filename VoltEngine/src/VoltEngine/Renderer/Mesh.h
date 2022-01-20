#pragma once
#include <string>

#include "VoltEngine/Core/Core.h"
#include "VoltEngine/AssetManager/Asset.h"

namespace Volt
{
    class IMaterial;
    class IVertexArray;
    struct SVertexData;

    class IMesh : public IAsset
    {
    DECLARE_DERIVED_CLASS(IMesh, IAsset);
    public:
        static SharedPtr<IMesh> Create();

        virtual const SharedPtr<IVertexArray>& GetVertexArray() const = 0;
        virtual const SharedPtr<IMaterial>& GetMaterial() const = 0;
        /** Profiling purposes */
        virtual uint32_t GetNumTriangles() const = 0;
    };
}
