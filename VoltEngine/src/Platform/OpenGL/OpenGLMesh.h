#pragma once
#include "VoltEngine/Renderer/Mesh.h"
#include "VoltEngine/Renderer/VertexData.h"

namespace Volt
{
    class COpenGLVertexArray;

    class COpenGLMesh : public IMesh
    {
    public:
        COpenGLMesh() = default;

        virtual const SharedPtr<IVertexArray>& GetVertexArray() const override;
        virtual const std::vector<SVertexData>& GetVertexData() const override { return m_vertexData; }
        virtual const std::vector<uint32_t>& GetIndexData() const override { return m_indexData; }
        virtual uint32_t GetNumTriangles() const override { return static_cast<uint32_t>(m_indexData.size()) / 3; }
        virtual const SharedPtr<IMaterial>& GetMaterial() const override { return m_material; }

        virtual void Load(const std::string& filepath) override;
        virtual void Unload() override;

    protected:
        void CreateBuffers();
    private:
        SharedPtr<IVertexArray> m_vertexArray {nullptr};
        std::vector<SVertexData> m_vertexData;
        std::vector<uint32_t> m_indexData;
        SharedPtr<IMaterial> m_material {nullptr};
    };
}
