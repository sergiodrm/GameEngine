// Test shader

#type vertex

#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_ModelTransform;

out vec3 v_Color;
out vec2 v_TexCoord;

void main()
{
	gl_Position = u_ViewProjection * u_ModelTransform * vec4(a_Position, 1.0);
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
}

#type fragment
#version 460 core

layout (location = 0) out vec4 color;

in vec3 v_Color;
in vec2 v_TexCoord;

uniform sampler2D texID;

void main()
{
	color = texture(texID, v_TexCoord);
}