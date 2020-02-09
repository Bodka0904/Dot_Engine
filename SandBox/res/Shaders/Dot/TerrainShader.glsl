#type vertex
#version 430 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Binormal;
layout(location = 4) in vec2 a_TexCoord;


layout(std140, binding = 0) uniform o_CameraData
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec3 ViewPos;
};

uniform mat4 u_ModelMatrix;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_ViewPos;
out vec3 v_WorldPos;

void main()
{
	vec4 WorldPos = u_ModelMatrix * vec4(a_Position.xyz, 1.0);
	vec4 Position = ViewProjectionMatrix * WorldPos;

	gl_Position = Position;

	v_FragPos = Position.xyz;
	v_Normal = mat3(transpose(inverse(u_ModelMatrix))) * a_Normal.xyz;
	v_TexCoord = a_TexCoord;
	v_ViewPos = ViewPos;
	v_WorldPos = WorldPos.xyz;
}


#type fragment
#version 430 core

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_ViewPos;
in vec3 v_WorldPos;

layout(std140, binding = 1) uniform o_Light
{
	vec4 LightPosition;
	vec4 LightColor;
	float LightStrength;
};


uniform float u_Radius;
uniform vec2 u_BrushPosition;

uniform sampler2D u_Texture;

out vec4 color;

const vec3 c_BrushColor = vec3(0.2, 0.7, 1.0);
float c_AmbientStrength = 0.8;
float c_SpecularStrength = 0.5;


void main()
{
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(LightPosition.xyz - v_FragPos);

	vec3 ambient = c_AmbientStrength * LightColor.xyz;
	vec3 viewDir = normalize(v_ViewPos - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	float diff = max(dot(norm, lightDir), 0.0);

	vec3 specular = c_SpecularStrength * spec * LightColor.xyz;
	vec3 diffuse = diff * LightColor.xyz;

	vec3 result = (ambient + diffuse + specular) * LightStrength;

	float dist = length(u_BrushPosition - v_WorldPos.xz);

	if (dist < u_Radius)
		result += c_BrushColor;

	color = texture(u_Texture, v_TexCoord) * vec4(result, 1.0);

}