#include "OpenGLMaterial.h"

#include "VoltEngine/Renderer/Shader.h"
#include "VoltEngine/Renderer/ShaderUniforms.h"
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
        shader->SetFloat4(ShaderUniforms::MaterialAmbientName, m_ambient);
        shader->SetFloat4(ShaderUniforms::MaterialDiffuseName, m_diffuse);
        shader->SetFloat4(ShaderUniforms::MaterialSpecularName, m_specular);
        shader->SetFloat(ShaderUniforms::MaterialShininessName, m_shininess);
        if (m_texture && m_useTexture)
        {
            shader->SetInt(ShaderUniforms::TextureAvailableFlagName, EUniformFlag::True);
            m_texture->Bind();
        }
        else
        {
            shader->SetInt(ShaderUniforms::TextureAvailableFlagName, EUniformFlag::False);
        }
    }
}
