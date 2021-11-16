#pragma once

#include <glm/glm.hpp>

namespace Volt
{
    struct SVertexData
    {
        glm::vec3 Position {0.f};
        glm::vec4 Color {1.f};
        glm::vec2 TexCoords {0.f};
        uint32_t TexIndex {0};

        const float* GetData() const { return &Position[0]; }

        static CBufferLayout GetStaticBufferLayout()
        {
            return {
                    {EShaderDataType::Float3, "a_Position"},
                    {EShaderDataType::Float4, "a_Color"},
                    {EShaderDataType::Float2, "a_TexCoords"},
                    {EShaderDataType::Float, "a_TexIndex"}
                };
        }
    };
}
