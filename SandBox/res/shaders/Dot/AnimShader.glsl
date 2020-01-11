#type vertex
#version 430 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in ivec4 a_BoneIDs;
layout(location = 4) in vec4 a_Weights;


out vec2 v_TexCoord;
out vec3 v_FragPos;
out vec3 v_Normal;
out vec3 v_WorldPos;
out vec3 v_ViewPos;

layout(std140, binding = 0) uniform o_CameraData
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec3 ViewPos;
};


const int MAX_BONES = 100;
uniform mat4 u_gBones[MAX_BONES];
uniform mat4 u_ModelMatrix;


void main()
{
	mat4 BoneTransform = u_gBones[a_BoneIDs[0]] * a_Weights[0];
	BoneTransform += u_gBones[a_BoneIDs[1]] * a_Weights[1];
	BoneTransform += u_gBones[a_BoneIDs[2]] * a_Weights[2];
	BoneTransform += u_gBones[a_BoneIDs[3]] * a_Weights[3];


	vec4 WorldPos = u_ModelMatrix * BoneTransform * vec4(a_Position, 1.0);

	vec4 Position = ViewProjectionMatrix * WorldPos;

	gl_Position = Position;

	v_FragPos = Position.xyz;
	v_Normal = mat3(transpose(inverse(u_ModelMatrix))) * a_Normal;
	v_TexCoord = a_TexCoord;
	v_ViewPos = ViewPos;
}


#type fragment
#version 430 core

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_ViewPos;

layout(std140, binding = 1) uniform o_Light
{
	vec4 LightPosition;
	vec4 LightColor;
	float LightStrength;
};

uniform sampler2D u_Texture;

out vec4 color;

float c_AmbientStrength = 0.1;
float c_SpecularStrength = 0.5;

float c_Constant = 1.0f;
float c_Linear = 0.09f;
float c_Quadratic = 0.032f;



float CalcPointLight()
{
	float distance = length(LightPosition.xyz - v_FragPos);
	float attenuation = 1.0 / (c_Constant + c_Linear * distance +
		c_Quadratic * (distance * distance));

	return attenuation;
}


vec3 CalcDirLight()
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

	return result;
}


void main()
{
	vec3 result = CalcDirLight();
	color = texture(u_Texture, v_TexCoord) * vec4(result, 1.0);
}


