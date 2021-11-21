#include "OpenGLMaterial.h"

#include "VoltEngine/Renderer/Shader.h"
#include "VoltEngine/Renderer/Texture.h"
#include "VoltEngine/Renderer/TextureManager.h"

namespace Volt
{
    COpenGLMaterial::COpenGLMaterial(const glm::vec4& ambient,
                                     const glm::vec4& diffuse,
                                     const glm::vec4& specular,
                                     float shininess,
                                     const std::string& textureFilepath)
        : m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess)
    {
        if (!textureFilepath.empty())
        {
            m_texture = CTextureManager::Get().Load(textureFilepath);
        }
    }

    void COpenGLMaterial::Prepare(const SharedPtr<IShader>& shader) const
    {
        shader->SetFloat4("u_Material.Ambient", m_ambient);
        shader->SetFloat4("u_Material.Diffuse", m_diffuse);
        shader->SetFloat4("u_Material.Specular", m_specular);
        shader->SetFloat("u_Material.Shininess", m_shininess);
        if (m_texture)
        {
            shader->SetInt("u_UseTexture", 1);
            m_texture->Bind();
        }
        else
        {
            shader->SetInt("u_UseTexture", 0);
        }
    }
}
