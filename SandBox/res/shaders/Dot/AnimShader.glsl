#type vertex
#version 330

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in ivec4 a_BoneIDs;
layout(location = 4) in vec4 a_Weights;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_WorldPos;

layout(std140) uniform camera_data
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
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


	gl_Position = ViewProjectionMatrix * u_ModelMatrix * BoneTransform * vec4(a_Position, 1.0);
	v_TexCoord = a_TexCoord;
	vec4 Normal = BoneTransform * vec4(a_Normal, 0.0);

}


#type fragment
#version 330 core

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

out vec4 color;

void main()
{
	color = texture(u_Texture, v_TexCoord);
}


