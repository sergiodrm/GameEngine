#pragma once

#include <glm/glm.hpp>

#include "Buffer.h"

namespace Volt
{
    struct SVertexData
    {
        glm::vec3 Position {0.f};
        glm::vec3 Normal {0.f};
        glm::vec4 Color {1.f};
        glm::vec2 TexCoords {0.f};

        const float* GetData() const { return &Position[0]; }

        static const CBufferLayout& GetStaticBufferLayout()
        {
            static const CBufferLayout layout {
                    {EShaderDataType::Float3, "a_Position"},
                    {EShaderDataType::Float3, "a_Normal"},
                    {EShaderDataType::Float4, "a_Color"},
                    {EShaderDataType::Float2, "a_TexCoords"}
                };
            return layout;
        }
    };
}
