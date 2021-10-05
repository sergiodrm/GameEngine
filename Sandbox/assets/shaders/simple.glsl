// Test shader

#type vertex

#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;

uniform mat4 u_ViewProjection;

out vec3 v_Color;

void main()
{
	gl_Position = vec4(a_Position, 1.0);
	v_Color = a_Color;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 gl_Color;

in vec3 v_Color;

uniform vec4 u_Color;

void main()
{
	gl_Color = u_Color;
//	gl_FragColor = vec4(v_Color, 1.0);
}