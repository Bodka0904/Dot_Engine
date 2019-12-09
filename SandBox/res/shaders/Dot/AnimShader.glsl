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


uniform vec2 u_ClipDistance;
const int MAX_BONES = 100;
uniform mat4 u_gBones[MAX_BONES];
uniform mat4 u_ModelMatrix;

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
	v_Normal = (ViewProjectionMatrix * u_ModelMatrix * BoneTransform * vec4(a_Normal, 0.0)).xyz;
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
uniform float u_AmbientStrength;

uniform sampler2D u_Texture;

out vec4 color;

void main()
{
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(u_LightPosition - v_FragPos);

	vec3 ambient = u_LightStrength * u_LightColor;
	vec3 viewDir = normalize(v_ViewPos - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	float diff = max(dot(norm, lightDir), 0.0);

	vec3 specular = u_LightStrength * spec * u_LightColor;
	vec3 diffuse = diff * u_LightColor;
	vec3 result =  (ambient + diffuse + specular);

	color = texture(u_Texture, v_TexCoord) * vec4(result, 1.0);
}

