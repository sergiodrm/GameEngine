#include "Shader.h"

#include "RendererAPI.h"
#include "VoltEngine/Core/Log.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Volt
{
    SharedPtr<IShader> IShader::Create(const std::string& filepath)
    {
        switch (IRendererAPI::GetAPI())
        {
            case IRendererAPI::API::OpenGL: return CreateSharedPtr<COpenGLShader>(filepath);
            default:
                VOLT_ASSERT(false, "Unknown RendererAPI");
        }
        return nullptr;
    }

    SharedPtr<IShader> IShader::Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
    {
        switch (IRendererAPI::GetAPI())
        {
            case IRendererAPI::API::OpenGL: return CreateSharedPtr<COpenGLShader>(name, vertexSource, fragmentSource);
            default:
                VOLT_ASSERT(false, "Unknown RendererAPI");
        }
        return nullptr;
    }

    void CShaderLibrary::Add(const SharedPtr<IShader>& shader)
    {
        Add(shader->GetName(), shader);
    }

    void CShaderLibrary::Add(const std::string& name, const SharedPtr<IShader>& shader)
    {
        VOLT_ASSERT(!Exists(name), "Shader already exists");
        m_shaders[name] = shader;
    }

    SharedPtr<IShader> CShaderLibrary::Load(const std::string& filepath)
    {
        const SharedPtr<IShader> shader = IShader::Create(filepath);
        Add(shader);
        return shader;
    }

    SharedPtr<IShader> CShaderLibrary::Load(const std::string& name, const std::string& filepath)
    {
        const SharedPtr<IShader> shader = IShader::Create(filepath);
        Add(name, shader);
        return shader;
    }

    SharedPtr<IShader> CShaderLibrary::Get(const std::string& name) const
    {
        VOLT_ASSERT(Exists(name), "{0} does not exist in the shader library.", name.c_str());
        return m_shaders.at(name);
    }

    bool CShaderLibrary::Exists(const std::string& name) const
    {
        return m_shaders.find(name) != m_shaders.end();
    }
}
