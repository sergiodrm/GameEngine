#pragma once
#include <vector>

#include "VoltEngine/Core/Core.h"


namespace Volt
{
    class IVertexArray
    {
    public:
        static SharedPtr<IVertexArray> Create();

        virtual ~IVertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(const SharedPtr<class IVertexBuffer>& vertexBuffer) = 0;
        virtual void SetIndexBuffer(const SharedPtr<class IIndexBuffer>& indexBuffer) = 0;

        virtual const std::vector<SharedPtr<IVertexBuffer>>& GetVertexBuffer() const = 0;
        virtual const SharedPtr<IIndexBuffer>& GetIndexBuffer() const = 0;
    };
}
