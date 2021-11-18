#include "OpenGLMesh.h"

#include "OpenGLVertexArray.h"
#include "VoltEngine/Renderer/Buffer.h"
#include "VoltEngine/Renderer/VertexData.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "VoltEngine/Renderer/Material.h"

namespace Volt
{
    COpenGLMesh::COpenGLMesh(const std::string& filepath)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn;
        std::string err;

        const uint32_t baseDirEnd = filepath.find_last_of('/');
        const std::string baseDir = filepath.substr(0, baseDirEnd);

        if (!LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str(), baseDir.c_str()))
        {
            VOLT_LOG(Error, "Couldn't find file {0}", filepath.c_str());
            return;
        }

        if (!warn.empty())
        {
            VOLT_LOG(Warning, "tinyobjloader WARNING: %s", warn.c_str());
        }
        if (!err.empty())
        {
            VOLT_LOG(Error, "tinyobjloader ERROR: %s", err.c_str());
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
        if (!materials.empty())
        {
            const std::string textureFilepath = baseDir + "/" + materials[0].ambient_texname;
            m_material = IMaterial::Create(
                                           {materials[0].ambient[0], materials[0].ambient[1], materials[0].ambient[2], 1.f},
                                           {materials[0].diffuse[0], materials[0].diffuse[1], materials[0].diffuse[2], 1.f},
                                           {materials[0].specular[0], materials[0].specular[1], materials[0].specular[2], 1.f},
                                           textureFilepath
                                          );
        }
        else
        {
            m_material = IMaterial::Create();
        }

        CreateBuffers();
    }

    COpenGLMesh::COpenGLMesh(const std::vector<SVertexData>& vertices, const std::vector<uint32_t>& indices)
        : m_vertexData(vertices), m_indexData(indices)
    {
        CreateBuffers();
    }

    const SharedPtr<IVertexArray>& COpenGLMesh::GetVertexArray() const { return m_vertexArray; }

    void COpenGLMesh::CreateBuffers()
    {
        // Vertices
        const SVertexData* vertexDataPtr = m_vertexData.data();
        const float* verticesPtr = vertexDataPtr->GetData();
        SharedPtr<IVertexBuffer> vertexBuffer = IVertexBuffer::Create(verticesPtr, m_vertexData.size() * sizeof(SVertexData));
        vertexBuffer->SetLayout(SVertexData::GetStaticBufferLayout());

        // Indices
        const SharedPtr<IIndexBuffer> indexBuffer = IIndexBuffer::Create(m_indexData.data(), m_indexData.size());

        // Vertex Array
        m_vertexArray = IVertexArray::Create();
        m_vertexArray->AddVertexBuffer(vertexBuffer);
        m_vertexArray->SetIndexBuffer(indexBuffer);
    }
}
