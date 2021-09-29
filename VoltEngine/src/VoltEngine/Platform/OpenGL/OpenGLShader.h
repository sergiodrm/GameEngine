#pragma once
#include "VoltEngine/Renderer/Shader.h"

namespace Volt
{
    class COpenGLShader : public IShader
    {
    public:
        COpenGLShader(const std::string& filepath);
        COpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
        virtual ~COpenGLShader() override;

        /** Begin Shader interface */
        virtual void Bind() override;
        virtual void Unbind() override;
        virtual void SetInt(const std::string& name, int32_t value) override;
        virtual void SetIntArray(const std::string& name, int32_t* values, uint32_t count) override;
        virtual void SetFloat(const std::string& name, float value) override;
        virtual void SetFloat2(const std::string& name, const glm::vec2& values) override;
        virtual void SetFloat3(const std::string& name, const glm::vec3& values) override;
        virtual void SetFloat4(const std::string& name, const glm::vec4& values) override;
        virtual void SetMat3(const std::string& name, const glm::mat3& values) override;
        virtual void SetMat4(const std::string& name, const glm::mat4& values) override;
        virtual const std::string& GetName() const override { return m_name; }
        /** End Shader interface */
    private:
        static std::string ReadFile(const std::string& filepath);
        static std::unordered_map<uint32_t, std::string> PreprocessSource(const std::string& rawSource);
        void Compile(const std::unordered_map<uint32_t, std::string>& shaderSources);

    private:
        uint32_t m_rendererID;
        std::string m_name;
    };
}
