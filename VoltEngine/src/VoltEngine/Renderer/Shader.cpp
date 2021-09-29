#include "Shader.h"

#include "RendererAPI.h"
#include "VoltEngine/Core/Log.h"
#include "VoltEngine/Platform/OpenGL/OpenGLShader.h"

namespace Volt
{
    Ref<IShader> IShader::Create(const std::string& filepath)
    {
        switch (IRendererAPI::GetAPI())
        {
            case IRendererAPI::API::OpenGL: return CreateRef<COpenGLShader>(filepath);
            default:
                VOLT_ASSERT(false, "Unknown RendererAPI");
        }
        return nullptr;
    }

    Ref<IShader> IShader::Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
    {
        switch (IRendererAPI::GetAPI())
        {
            case IRendererAPI::API::OpenGL: return CreateRef<COpenGLShader>(name, vertexSource, fragmentSource);
            default:
                VOLT_ASSERT(false, "Unknown RendererAPI");
        }
        return nullptr;
    }

    void CShaderLibrary::Add(const Ref<IShader>& shader)
    {
        Add(shader->GetName(), shader);
    }

    void CShaderLibrary::Add(const std::string& name, const Ref<IShader>& shader)
    {
        VOLT_ASSERT(!Exists(name), "Shader already exists");
        m_shaders[name] = shader;
    }

    Ref<IShader> CShaderLibrary::Load(const std::string& filepath)
    {
        const Ref<IShader> shader = IShader::Create(filepath);
        Add(shader);
        return shader;
    }

    Ref<IShader> CShaderLibrary::Load(const std::string& name, const std::string& filepath)
    {
        const Ref<IShader> shader = IShader::Create(filepath);
        Add(name, shader);
        return shader;
    }

    Ref<IShader> CShaderLibrary::Get(const std::string& name) const
    {
        VOLT_ASSERT(Exists(name), "{0} does not exist in the shader library.", name.c_str());
        return m_shaders.at(name);
    }

    bool CShaderLibrary::Exists(const std::string& name) const
    {
        return m_shaders.find(name) != m_shaders.end();
    }
}
