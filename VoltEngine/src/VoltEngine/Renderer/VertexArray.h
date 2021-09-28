#pragma once
#include <vector>

#include "VoltEngine/Core/Core.h"


namespace Volt
{
    class IVertexArray
    {
    public:
        static Ref<IVertexArray> Create();

        virtual ~IVertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(const Ref<class IVertexBuffer>& vertexBuffer) = 0;
        virtual void SetIndexBuffer(const Ref<class IIndexBuffer>& indexBuffer) = 0;

        virtual const std::vector<Ref<IVertexBuffer>>& GetVertexBuffer() const = 0;
        virtual const Ref<IIndexBuffer>& GetIndexBuffer() const = 0;
    };
}
