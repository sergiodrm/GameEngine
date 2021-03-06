#pragma once
#include <string>

#include <glm/glm.hpp>

#include "VoltEngine/AssetManager/Asset.h"
#include "VoltEngine/Core/Core.h"

namespace Volt
{
    class ITexture;
    class IShader;

    class IMaterial : public IAsset
    {
    DECLARE_DERIVED_CLASS(IMaterial, IAsset)
    public:
        static SharedPtr<IMaterial> Create(const glm::vec4& ambient = glm::vec4(1.f),
                                           const glm::vec4& diffuse = glm::vec4(1.f),
                                           const glm::vec4& specular = glm::vec4(1.f),
                                           float shininess = 1.f,
                                           const std::string& textureFilepath = {});

        virtual void Prepare(const SharedPtr<IShader>& shader) const = 0;

        virtual const glm::vec4& GetAmbient() const = 0;
        virtual const glm::vec4& GetDiffuse() const = 0;
        virtual const glm::vec4& GetSpecular() const = 0;
        virtual float GetShininess() const = 0;
        virtual bool IsUsingTexture() const = 0;
        virtual void SetTexture(const SharedPtr<ITexture>& texture) = 0;
        virtual void SetTexture(const std::string& filepath) = 0;

        virtual void SetAmbient(const glm::vec4& ambient) = 0;
        virtual void SetDiffuse(const glm::vec4& diffuse) = 0;
        virtual void SetSpecular(const glm::vec4& specular) = 0;
        virtual void SetShininess(float shininess) = 0;
        virtual void UseTexture(bool use) = 0;
    };
}
