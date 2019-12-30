#type vertex
#version 330

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in ivec4 a_BoneIDs;
layout(location = 4) in vec4 a_Weights;


out vec2 v_TexCoord;
out vec3 v_FragPos;
out vec3 v_Normal;
out vec3 v_WorldPos;
out vec3 v_ViewPos;

layout(std140) uniform camera_data
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec3 ViewPos;
};


const int MAX_BONES = 100;
uniform mat4 u_gBones[MAX_BONES];
uniform mat4 u_ModelMatrix;
uniform vec2 u_ClipDistance;

void main()
{
	mat4 BoneTransform = u_gBones[a_BoneIDs[0]] * a_Weights[0];
	BoneTransform += u_gBones[a_BoneIDs[1]] * a_Weights[1];
	BoneTransform += u_gBones[a_BoneIDs[2]] * a_Weights[2];
	BoneTransform += u_gBones[a_BoneIDs[3]] * a_Weights[3];

	vec4 plane = vec4(0, u_ClipDistance.x, 0, u_ClipDistance.y);
	vec4 WorldPos = u_ModelMatrix * BoneTransform * vec4(a_Position, 1.0);
	gl_ClipDistance[0] = dot(WorldPos, plane);

	vec4 Position = ViewProjectionMatrix * WorldPos;

	gl_Position = Position;

	v_FragPos = Position.xyz;
	v_Normal = mat3(transpose(inverse(u_ModelMatrix))) * a_Normal;
	v_TexCoord = a_TexCoord;
	v_ViewPos = ViewPos;
}


#type fragment
#version 330 core

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_ViewPos;

uniform vec3 u_LightPosition;
uniform vec3 u_LightColor;
uniform float u_LightStrength;

uniform sampler2D u_Texture;

out vec4 color;

float c_AmbientStrength = 0.3;
float c_SpecularStrength = 0.5;
float c_Constant = 1.0f;
float c_Linear = 0.09f;
float c_Quadratic = 0.032f;



float CalcPointLight()
{
	float distance = length(u_LightPosition - v_FragPos);
	float attenuation = 1.0 / (c_Constant + c_Linear * distance +
		c_Quadratic * (distance * distance));

	return attenuation;
}


vec3 CalcDirLight()
{
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(u_LightPosition - v_FragPos);

	vec3 ambient = c_AmbientStrength * u_LightColor;
	vec3 viewDir = normalize(v_ViewPos - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	float diff = max(dot(norm, lightDir), 0.0);

	vec3 specular = c_SpecularStrength * spec * u_LightColor;
	vec3 diffuse = diff * u_LightColor;

	float attenuation = CalcPointLight();
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = (ambient + diffuse + specular) * u_LightStrength;

	return result;
}


void main()
{
	vec3 result = CalcDirLight();
	color = texture(u_Texture, v_TexCoord) * vec4(result, 1.0);
}


