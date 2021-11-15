#pragma once

#include <glm/glm.hpp>

namespace Volt
{
    struct SVertexData
    {
        glm::vec3 Position {0.f};
        glm::vec4 Color {1.f};

        const float* GetData() const { return &Position[0]; }
    };
}
