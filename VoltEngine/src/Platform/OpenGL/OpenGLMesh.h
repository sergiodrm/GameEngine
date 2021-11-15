#pragma once
#include "VoltEngine/Renderer/Mesh.h"

namespace Volt
{
    class COpenGLVertexArray;

    class COpenGLMesh : public IMesh
    {
    public:
        COpenGLMesh(const std::string& filepath);
        COpenGLMesh(const std::vector<SVertexData>& vertices, const std::vector<uint32_t>& indices);

        virtual const SharedPtr<IVertexArray>& GetVertexArray() const override;

    private:
        SharedPtr<IVertexArray> m_vertexArray {nullptr};
    };
}
