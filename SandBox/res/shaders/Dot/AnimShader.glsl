#type vertex
#version 430 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in ivec4 a_BoneIDs;
layout(location = 4) in vec4 a_Weights;


out vec2 v_TexCoord;
out vec3 v_SurfaceNormal;
out vec3 v_CameraVector;
out vec4 v_WorldPos;

out vec3 v_Normal;

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

	mat4 Model = u_ModelMatrix * BoneTransform;
	vec4 WorldPos = Model * vec4(a_Position, 1.0);

	gl_Position = ViewProjectionMatrix * WorldPos;

	v_TexCoord = a_TexCoord;
	v_SurfaceNormal = vec4(u_ModelMatrix * vec4(a_Normal, 0.0)).xyz;
	v_WorldPos = WorldPos;
	v_CameraVector = vec4(inverse(ViewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - WorldPos.xyz;


	v_Normal = mat3(transpose(inverse(Model))) * a_Normal;
}


#type fragment
#version 430 core

in vec2 v_TexCoord;
in vec3 v_SurfaceNormal;
in vec3 v_CameraVector;
in vec4 v_WorldPos;

in vec3 v_Normal;

layout(std140, binding = 1) uniform o_Light
{
	vec4 LightPosition;
	vec4 LightColor;
	float LightStrength;
};
uniform float u_Reflectivity;

uniform sampler2D u_Texture;

out vec4 color;

vec3 CalculateLight()
{
	float ambientStrength = LightStrength;
	vec3 ambient = ambientStrength * LightColor.xyz;

	// diffuse 
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(LightPosition.xyz - v_WorldPos.xyz);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * LightColor.xyz;

	// specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(v_CameraVector);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = u_Reflectivity * spec * LightColor.xyz;

	return (ambient + diffuse + specular);
}

void main()
{
	color = texture(u_Texture, v_TexCoord) * vec4(CalculateLight(),1.0);
}


