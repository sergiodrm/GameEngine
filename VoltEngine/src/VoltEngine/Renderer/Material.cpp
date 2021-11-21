#include "Material.h"

#include "RendererAPI.h"
#include "VoltEngine/Core/Log.h"
#include "Platform/OpenGL/OpenGLMaterial.h"

namespace Volt
{
    SharedPtr<IMaterial> IMaterial::Create(const glm::vec4& ambient,
                                           const glm::vec4& diffuse,
                                           const glm::vec4& specular,
                                           float shininess,
                                           const std::string& textureFilepath)
    {
        switch (IRendererAPI::GetAPI())
        {
            case IRendererAPI::API::OpenGL: return CreateSharedPtr<COpenGLMaterial>(ambient, diffuse, specular, shininess, textureFilepath);
        }
        VOLT_ASSERT(false, "Unsupported Renderer API!");
        return nullptr;
    }
}
