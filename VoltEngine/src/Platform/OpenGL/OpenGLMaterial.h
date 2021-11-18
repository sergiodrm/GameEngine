#pragma once

#include "VoltEngine/Renderer/Material.h"

namespace Volt
{
    class ITexture;

    class COpenGLMaterial : public IMaterial
    {
    public:
        COpenGLMaterial(const glm::vec4& ambient = glm::vec4(1.f),
                        const glm::vec4& diffuse = glm::vec4(1.f),
                        const glm::vec4& specular = glm::vec4(1.f),
                        const std::string& textureFilepath = {});
        virtual void Prepare(const SharedPtr<IShader>& shader) const override;
        virtual const glm::vec4& GetAmbient() const override { return m_ambient; }
        virtual const glm::vec4& GetDiffuse() const override { return m_diffuse; }
        virtual const glm::vec4& GetSpecular() const override { return m_specular; }
        virtual void SetAmbient(const glm::vec4& ambient) override { m_ambient = ambient; }
        virtual void SetDiffuse(const glm::vec4& diffuse) override { m_diffuse = diffuse; }
        virtual void SetSpecular(const glm::vec4& specular) override { m_specular = specular; }
    private:
        glm::vec4 m_ambient;
        glm::vec4 m_diffuse;
        glm::vec4 m_specular;
        SharedPtr<ITexture> m_texture;
    };
}
