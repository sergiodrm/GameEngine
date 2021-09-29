#pragma once
#include "glm/glm.hpp"
#include "VoltEngine/Core/Core.h"

namespace Volt
{
    class IShader
    {
    public:
        static Ref<IShader> Create(const std::string& filepath);
        static Ref<IShader> Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);

        virtual ~IShader() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void SetInt(const std::string& name, int32_t value) = 0;
        virtual void SetIntArray(const std::string& name, int32_t* values, uint32_t count) = 0;

        virtual void SetFloat(const std::string& name, float value) = 0;
        virtual void SetFloat2(const std::string& name, const glm::vec2& values) = 0;
        virtual void SetFloat3(const std::string& name, const glm::vec3& values) = 0;
        virtual void SetFloat4(const std::string& name, const glm::vec4& values) = 0;

        virtual void SetMat3(const std::string& name, const glm::mat3& values) = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4& values) = 0;

        virtual const std::string& GetName() const = 0;
    };

    class CShaderLibrary
    {
    public:
        void Add(const Ref<IShader>& shader);
        void Add(const std::string& name, const Ref<IShader>& shader);

        Ref<IShader> Load(const std::string& filepath);
        Ref<IShader> Load(const std::string& name, const std::string& filepath);

        Ref<IShader> Get(const std::string& name) const;

        bool Exists(const std::string& name) const;
    private:
        std::unordered_map<std::string, Ref<IShader>> m_shaders;
    };
}
