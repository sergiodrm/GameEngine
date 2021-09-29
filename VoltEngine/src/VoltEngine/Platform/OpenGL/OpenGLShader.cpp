#include "OpenGLShader.h"
#include "VoltEngine/VoltEngine.h"
#include <fstream>
#include <sstream>
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

namespace Volt
{
    enum : uint32_t
    {
        Vertex,
        Fragment
    };

    COpenGLShader::COpenGLShader(const std::string& filepath)
    {
        const std::string rawSource = ReadFile(filepath);
        const std::unordered_map<uint32_t, std::string> shaderSources = PreprocessSource(rawSource);
        Compile(shaderSources);

        // Get name
        size_t lastSlash = filepath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        const size_t lastDot = filepath.rfind('.');
        const size_t count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
        m_name = filepath.substr(lastSlash, count);
    }

    COpenGLShader::COpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
        : m_name(name)
    {
        Compile({{Vertex, vertexSource}, {Fragment, fragmentSource}});
    }

    COpenGLShader::~COpenGLShader()
    {
        glDeleteProgram(m_rendererID);
    }

    void COpenGLShader::Bind()
    {
        glUseProgram(m_rendererID);
    }

    void COpenGLShader::Unbind()
    {
        glUseProgram(0);
    }

    void COpenGLShader::SetInt(const std::string& name, int32_t value)
    {
        const GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniform1i(location, value);
    }

    void COpenGLShader::SetIntArray(const std::string& name, int32_t* values, uint32_t count)
    {
        const GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniform1iv(location, count, values);
    }

    void COpenGLShader::SetFloat(const std::string& name, float value)
    {
        const GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniform1f(location, value);
    }

    void COpenGLShader::SetFloat2(const std::string& name, const glm::vec2& values)
    {
        const GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniform2f(location, values.x, values.y);
    }

    void COpenGLShader::SetFloat3(const std::string& name, const glm::vec3& values)
    {
        const GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniform3f(location, values.x, values.y, values.z);
    }

    void COpenGLShader::SetFloat4(const std::string& name, const glm::vec4& values)
    {
        const GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniform4f(location, values.x, values.y, values.z, values.w);
    }

    void COpenGLShader::SetMat3(const std::string& name, const glm::mat3& values)
    {
        const GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(values));
    }

    void COpenGLShader::SetMat4(const std::string& name, const glm::mat4& values)
    {
        const GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(values));
    }

    std::string COpenGLShader::ReadFile(const std::string& filepath)
    {
        std::string rawSource;
        std::ifstream stream(filepath, std::ios::in | std::ios::binary);
        if (stream)
        {
            stream.seekg(0, std::ios::end);
            rawSource.resize(stream.tellg());
            stream.seekg(0, std::ios::beg);
            stream.read(&rawSource[0], rawSource.size());
            stream.close();
        }
        else
        {
            VOLT_LOG(Error, "Could not open file '{0}'", filepath);
        }
        return rawSource;
    }

    std::unordered_map<uint32_t, std::string> COpenGLShader::PreprocessSource(const std::string& rawSource)
    {
        std::unordered_map<uint32_t, std::string> shaderSources;
        const char* typeToken = "#type";
        const size_t typeTokenLength = strlen(typeToken);
        size_t pos = rawSource.find(typeToken, 0);
        while (pos != std::string::npos)
        {
            const size_t eol = rawSource.find_first_of("\r\n", pos);
            VOLT_ASSERT(eol != std::string::npos, "Syntax error");
            const size_t begin = pos + typeTokenLength + 1;
            std::string type = rawSource.substr(begin, eol - begin);
            VOLT_ASSERT(type == "vertex" || type == "fragment", "Unknown type of shader.");

            const size_t nextLinePos = rawSource.find_first_not_of("\r\n");
            pos = rawSource.find(typeToken, nextLinePos);

            shaderSources[type == "vertex" ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER] =
                rawSource.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? rawSource.size() - 1 : nextLinePos));
        }
        return shaderSources;
    }

    void COpenGLShader::Compile(const std::unordered_map<uint32_t, std::string>& shaderSources)
    {
        const GLuint program = glCreateProgram();
        VOLT_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now");
        std::array<GLenum, 2> glShaderIDs;
        int glShaderIDIndex = 0;
        for (const std::pair<GLenum, std::string>& it : shaderSources)
        {
            const GLenum type = it.first;
            const std::string& source = it.second;

            const GLuint shader = glCreateShader(type);
            const GLchar* rawSource = source.c_str();
            glShaderSource(shader, 1, &rawSource, nullptr);

            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                // The maxLength includes the NULL character
                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                // We don't need the shader and program anymore.
                glDeleteProgram(program);

                for (GLenum id : glShaderIDs)
                {
                    glDeleteShader(id);
                }
                // Use the infoLog as you see fit.
                VOLT_LOG(Error, "{0}", infoLog.data());
                VOLT_ASSERT(false, "Shader compilation failed");
                break;
            }

            glAttachShader(program, shader);
            glShaderIDs[glShaderIDIndex++] = shader;
        }

        // Link our program
        glLinkProgram(program);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(program);
            // Don't leak shaders either.
            for (GLenum id : glShaderIDs)
            {
                glDeleteShader(id);
            }
            // Use the infoLog as you see fit.
            VOLT_LOG(Error, "{0}", infoLog.data());
            VOLT_ASSERT(false, "Shader compilation failed");

            // In this simple program, we'll just leave
            return;
        }

        // Always detach shaders after a successful link.
        for (GLenum id : glShaderIDs)
        {
            glDetachShader(program, id);
        }

        // Save the program id
        m_rendererID = program;
    }
}
