#include "OpenGLMesh.h"

#include "OpenGLVertexArray.h"
#include "VoltEngine/Renderer/Buffer.h"
#include "VoltEngine/Renderer/VertexData.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "VoltEngine/Renderer/Material.h"

namespace Volt
{
    const SharedPtr<IVertexArray>& COpenGLMesh::GetVertexArray() const { return m_vertexArray; }

    void COpenGLMesh::Load()
    {
        const std::string& filepath = GetFilepath();
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn;
        std::string err;

        uint32_t baseDirEnd = static_cast<uint32_t>(filepath.find_last_of('/'));
        if (baseDirEnd >= filepath.length())
            baseDirEnd = static_cast<uint32_t>(filepath.find_last_of('\\'));
        const std::string baseDir = filepath.substr(0, baseDirEnd);

        if (!LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str(), baseDir.c_str()))
        {
            VOLT_LOG(Error, "Couldn't find file {0}", filepath.c_str());
            return;
        }

        if (!warn.empty())
        {
            VOLT_LOG(Warning, "tinyobjloader WARNING: {0}", warn.c_str());
        }
        if (!err.empty())
        {
            VOLT_LOG(Error, "tinyobjloader ERROR: {0}", err.c_str());
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
                if (!attrib.normals.empty())
                {
                    vertexData.Normal = {
                            attrib.normals[3 * index.normal_index + 0],
                            attrib.normals[3 * index.normal_index + 1],
                            attrib.normals[3 * index.normal_index + 2],
                        };
                }
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
            const std::string textureName = materials[0].ambient_texname.empty() ? materials[0].diffuse_texname : materials[0].ambient_texname;
            const std::string textureFilepath = textureName.empty() ? "" : baseDir + "/" + textureName;
            m_material = IMaterial::Create(
                                           {materials[0].ambient[0], materials[0].ambient[1], materials[0].ambient[2], 1.f},
                                           {materials[0].diffuse[0], materials[0].diffuse[1], materials[0].diffuse[2], 1.f},
                                           {materials[0].specular[0], materials[0].specular[1], materials[0].specular[2], 1.f},
                                           materials[0].shininess,
                                           textureFilepath
                                          );
        }
        else
        {
            m_material = IMaterial::Create();
        }

        CreateBuffers();
    }

    void COpenGLMesh::Unload()
    {
        m_vertexArray.reset();
        m_material.reset();
        m_indexData.clear();
        m_vertexData.clear();
    }

    void COpenGLMesh::CreateBuffers()
    {
        // Vertices
        const SVertexData* vertexDataPtr = m_vertexData.data();
        const float* verticesPtr = vertexDataPtr->GetData();
        SharedPtr<IVertexBuffer> vertexBuffer = IVertexBuffer::Create(verticesPtr, static_cast<uint32_t>(m_vertexData.size()) * sizeof(SVertexData));
        vertexBuffer->SetLayout(SVertexData::GetStaticBufferLayout());

        // Indices
        const SharedPtr<IIndexBuffer> indexBuffer = IIndexBuffer::Create(m_indexData.data(), m_indexData.size());

        // Vertex Array
        m_vertexArray = IVertexArray::Create();
        m_vertexArray->AddVertexBuffer(vertexBuffer);
        m_vertexArray->SetIndexBuffer(indexBuffer);
    }
}
