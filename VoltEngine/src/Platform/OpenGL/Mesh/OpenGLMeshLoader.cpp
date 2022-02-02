#include "OpenGLMeshLoader.h"

#include <future>
#include <tiny_obj_loader.h>

#include "OpenGLMesh.h"
#include "VoltEngine/AssetManager/AssetManager.h"
#include "VoltEngine/Core/Time.h"

namespace Volt
{
    COpenGLMeshLoader::COpenGLMeshLoader(IAsset* asset)
        : IAssetLoader(asset), m_meshAssetData() {}


    void COpenGLMeshLoader::LoadDataInAsset()
    {
        if (COpenGLMesh* asset = dynamic_cast<COpenGLMesh*>(GetAsset()))
        {
            asset->LoadData(m_meshAssetData);
        }
    }

    void COpenGLMeshLoader::Load(std::string filepath, bool async)
    {
        PROFILE_SCOPE(MeshLoaderLoad);

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
            VOLT_LOG(Warning, "tinyobjloader WARNING: {0}", warn.c_str());
        if (!err.empty())
            VOLT_LOG(Error, "tinyobjloader ERROR: {0}", err.c_str());


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
                m_meshAssetData.Vertices.push_back(vertexData);
                m_meshAssetData.Indices.push_back(static_cast<uint32_t>(m_meshAssetData.Indices.size()));
            }
        }
        if (!materials.empty())
        {
            const std::string textureName = materials[0].ambient_texname.empty() ? materials[0].diffuse_texname : materials[0].ambient_texname;
            const std::string textureFilepath = textureName.empty() ? "" : baseDir + "/" + textureName;
            m_meshAssetData.MaterialAssetData.MaterialName = materials[0].name;
            m_meshAssetData.MaterialAssetData.TextureFilepath = textureFilepath;
            m_meshAssetData.MaterialAssetData.Ambient = {materials[0].ambient[0], materials[0].ambient[1], materials[0].ambient[2], 1.f};
            m_meshAssetData.MaterialAssetData.Diffuse = {materials[0].diffuse[0], materials[0].diffuse[1], materials[0].diffuse[2], 1.f};
            m_meshAssetData.MaterialAssetData.Specular = {materials[0].specular[0], materials[0].specular[1], materials[0].specular[2], 1.f};
            m_meshAssetData.MaterialAssetData.Shininess = materials[0].shininess;
        }

        if (async)
        {
            CAssetManager::Get().PushLoadRequest(this);
        }
    }
}
