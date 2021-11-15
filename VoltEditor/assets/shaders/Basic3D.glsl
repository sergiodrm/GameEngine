#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 u_MVP;

out vec4 v_Color;

void main()
{
    gl_Position = u_MVP * vec4(a_Position, 1.f);
    v_Color = a_Color;
}

#type fragment
#version 460 core

layout (location = 0) out vec4 color;

in vec4 v_Color;

void main()
{
    color = v_Color;
}
