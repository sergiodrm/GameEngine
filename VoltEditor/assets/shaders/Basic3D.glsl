#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in vec2 a_TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_MVP;
uniform mat4 u_Model;

out vec4 v_Color;
out vec2 v_TexCoords;
out vec3 v_Normal;
out vec3 v_FragPos;

void main()
{
    gl_Position = u_MVP * vec4(a_Position, 1.f);
    v_Color = a_Color;
    v_TexCoords = a_TexCoords;
    v_FragPos = vec3(u_Model * vec4(a_Position, 1.f));
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
}

/*
----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------
*/

#type fragment
#version 460 core

struct SMaterial
{
    vec4 Ambient;
    vec4 Diffuse;
    vec4 Specular;
    float Shininess;
};

layout (location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoords;
in vec3 v_Normal;
in vec3 v_FragPos;

uniform vec3 u_ViewPos;

uniform int u_UseTexture;
uniform int u_UseLight;

uniform sampler2D u_Texture;

uniform SMaterial u_Material;

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;

uniform vec3 u_AmbientColor;
uniform float u_AmbientStrength;

vec3 CalculateDiffuseLight();
vec3 CalculateSpecularLight();
vec4 CalculateTexture();

void main()
{
    vec3 ambient = u_AmbientColor * u_AmbientStrength * vec3(u_Material.Ambient);
    vec3 diffuse = CalculateDiffuseLight();
    vec3 specular = CalculateSpecularLight();
    vec4 textureColor = CalculateTexture();
    color = textureColor * vec4(specular + diffuse + ambient, 1.0);
}


vec3 CalculateDiffuseLight()
{
    vec3 diffuse = vec3(0.0, 0.0, 0.0);
    if (u_UseLight == 1)
    {
        vec3 normal = normalize(v_Normal);
        vec3 lightDirection = normalize(u_LightPos - v_FragPos);
        float diff = max(dot(normal, lightDirection), 0.0);

        diffuse = diff * u_LightColor * vec3(u_Material.Diffuse);
    }
    return diffuse;
}

vec3 CalculateSpecularLight()
{
    vec3 specular = vec3(0.0, 0.0, 0.0);
    if (u_UseLight == 1)
    {
        vec3 normal = normalize(v_Normal);
        vec3 lightDirection = normalize(u_LightPos - v_FragPos);
        vec3 viewDir = normalize(u_ViewPos - v_FragPos);
        vec3 reflectDir = reflect(-lightDirection, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Shininess);
        specular = spec * u_LightColor * vec3(u_Material.Specular);
    }
    return specular;
}

vec4 CalculateTexture()
{
    vec4 textureColor = v_Color;
    if (u_UseTexture == 1)
    {
        textureColor = texture(u_Texture, v_TexCoords) * v_Color;
    }
    return textureColor;
}
