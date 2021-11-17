#include "OpenGLMesh.h"

#include "OpenGLVertexArray.h"
#include "VoltEngine/Renderer/Buffer.h"
#include "VoltEngine/Renderer/VertexData.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace Volt
{
    COpenGLMesh::COpenGLMesh(const std::string& filepath)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn;
        std::string err;

        if (!LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str()))
        {
            VOLT_LOG(Error, "Couldn't find file {0}", filepath.c_str());
            return;
        }

        for (const tinyobj::shape_t& shape : shapes)
        {
            for (const tinyobj::index_t& index : shape.mesh.indices)
            {
                SVertexData vertexData;
                vertexData.Position = {
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2],
                    };
                if (!attrib.colors.empty())
                {
                    vertexData.Color = {
                            attrib.colors[3 * index.vertex_index + 0],
                            attrib.colors[3 * index.vertex_index + 1],
                            attrib.colors[3 * index.vertex_index + 2],
                            1.f
                        };
                }
                if (!attrib.texcoords.empty())
                {
                    vertexData.TexCoords = {
                            attrib.texcoords[2 * index.texcoord_index + 0],
                            attrib.texcoords[2 * index.texcoord_index + 1]
                        };
                }
                m_vertexData.push_back(vertexData);
                m_indexData.push_back(m_indexData.size());
            }
        }
    }

    COpenGLMesh::COpenGLMesh(const std::vector<SVertexData>& vertices, const std::vector<uint32_t>& indices)
        : m_vertexData(vertices), m_indexData(indices)
    {
        // Vertices
        const SVertexData* vertexDataPtr = vertices.data();
        const float* verticesPtr = vertexDataPtr->GetData();
        SharedPtr<IVertexBuffer> vertexBuffer = IVertexBuffer::Create(verticesPtr, vertices.size() * sizeof(SVertexData));
        vertexBuffer->SetLayout(SVertexData::GetStaticBufferLayout());

        // Indices
        const SharedPtr<IIndexBuffer> indexBuffer = IIndexBuffer::Create(indices.data(), indices.size());

        // Vertex Array
        m_vertexArray = IVertexArray::Create();
        m_vertexArray->AddVertexBuffer(vertexBuffer);
        m_vertexArray->SetIndexBuffer(indexBuffer);
    }

    const SharedPtr<IVertexArray>& COpenGLMesh::GetVertexArray() const { return m_vertexArray; }
}
