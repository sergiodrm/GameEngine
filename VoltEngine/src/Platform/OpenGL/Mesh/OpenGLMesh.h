#pragma once
#include "VoltEngine/Renderer/Mesh.h"
#include "VoltEngine/Renderer/VertexData.h"

namespace Volt
{
    class COpenGLVertexArray;

    class COpenGLMesh : public IMesh
    {
    DECLARE_DERIVED_CLASS(COpenGLMesh, IMesh);
    public:
        COpenGLMesh() = default;
        virtual ~COpenGLMesh() override;

        /** Begin IAsset methods */
        virtual IAssetLoader* CreateLoader() override;
        virtual bool IsLoaded() const override;
        /** End IAsset methods */

        /** Begin IMesh methods */
        virtual const SharedPtr<IVertexArray>& GetVertexArray() const override;
        virtual uint32_t GetNumTriangles() const override { return m_numOfTriangles; }
        virtual const SharedPtr<IMaterial>& GetMaterial() const override { return m_material; }
        /** End IMesh methods */

    protected:
        void CreateBuffers();
    private:
        SharedPtr<IVertexArray> m_vertexArray {nullptr};
        SharedPtr<IMaterial> m_material {nullptr};
        uint32_t m_numOfTriangles {0};
        bool m_isLoaded {false};
    };
}
