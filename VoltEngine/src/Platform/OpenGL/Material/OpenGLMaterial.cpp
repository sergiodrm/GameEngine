#include "OpenGLMaterial.h"

#include "OpenGLMaterialLoader.h"
#include "VoltEngine/AssetManager/AssetManager.h"
#include "VoltEngine/Renderer/Shader.h"
#include "VoltEngine/Renderer/ShaderUniforms.h"
#include "VoltEngine/Renderer/Texture.h"

namespace Volt
{
    COpenGLMaterial::COpenGLMaterial(const glm::vec4& ambient,
                                     const glm::vec4& diffuse,
                                     const glm::vec4& specular,
                                     float shininess,
                                     const std::string& textureFilepath)
        : m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess)
    { }

    UniquePtr<IAssetLoader> COpenGLMaterial::CreateLoader()
    {
        return CreateUnique<COpenGLMaterialLoader>(this);
    }

    void COpenGLMaterial::Prepare(const SharedPtr<IShader>& shader) const
    {
        shader->SetFloat4(ShaderUniforms::MaterialAmbientName, m_ambient);
        shader->SetFloat4(ShaderUniforms::MaterialDiffuseName, m_diffuse);
        shader->SetFloat4(ShaderUniforms::MaterialSpecularName, m_specular);
        shader->SetFloat(ShaderUniforms::MaterialShininessName, m_shininess);
        if (m_texture && m_texture->IsLoaded() && m_useTexture)
        {
            shader->SetInt(ShaderUniforms::TextureAvailableFlagName, EUniformFlag::True);
            m_texture->Bind();
        }
        else
        {
            shader->SetInt(ShaderUniforms::TextureAvailableFlagName, EUniformFlag::False);
        }
    }

    void COpenGLMaterial::SetTexture(const SharedPtr<ITexture>& texture)
    {
        m_texture = texture;
    }

    void COpenGLMaterial::SetTexture(const std::string& filepath)
    {
        m_texture = CAssetManager::LoadAsset<ITexture>(filepath);
    }
}
