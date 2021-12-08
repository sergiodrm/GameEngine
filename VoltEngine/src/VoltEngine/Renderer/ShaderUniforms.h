#pragma once

namespace Volt
{
    namespace EUniformFlag
    {
        enum Type : uint32_t
        {
            False = 0,
            True = 1,
        };
    }

    namespace ShaderUniforms
    {
        // Matrices
        static constexpr const char* ViewProjectionMatrixName = "u_ViewProjection";
        static constexpr const char* MVPMatrixName = "u_MVP";
        static constexpr const char* ModelMatrixName = "u_Model";

        // Camera
        static constexpr const char* CameraPositionName = "u_ViewPos";

        // Flags
        static constexpr const char* TextureAvailableFlagName = "u_UseTexture";
        static constexpr const char* LightAvailableFlagName = "u_UseLight";

        // Lights
        static constexpr const char* LightPositionName = "u_LightPos";
        static constexpr const char* LightColorName = "u_LightColor";
        static constexpr const char* AmbientColorName = "u_AmbientColor";
        static constexpr const char* AmbientStrengthName = "u_AmbientStrength";

        // Material
        static constexpr const char* MaterialName = "u_Material";
        static constexpr const char* MaterialAmbientName = "u_Material.Ambient";
        static constexpr const char* MaterialDiffuseName = "u_Material.Diffuse";
        static constexpr const char* MaterialSpecularName = "u_Material.Specular";
        static constexpr const char* MaterialShininessName = "u_Material.Shininess";
    }
}
