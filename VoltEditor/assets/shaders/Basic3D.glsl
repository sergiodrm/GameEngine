#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_MVP;

out vec4 v_Color;
out vec2 v_TexCoords;

void main()
{
    gl_Position = u_MVP * vec4(a_Position, 1.f);
    v_Color = vec4(a_Position, 1.f);
    v_TexCoords = a_TexCoords;
}

#type fragment
#version 460 core

struct SMaterial
{
    vec4 Ambient;
    vec4 Diffuse;
    vec4 Specular;
};

layout (location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoords;

uniform int u_UseTexture;
uniform sampler2D u_Texture;
uniform SMaterial u_Material;

void main()
{
    if (u_UseTexture == 1)
    {
        color = texture(u_Texture, v_TexCoords);
    }
    else
    {
        color = v_Color;
    }
}
