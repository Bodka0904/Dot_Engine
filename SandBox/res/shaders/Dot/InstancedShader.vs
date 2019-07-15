#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in mat4 instanceModel;

layout (std140) uniform camera_data
{
	mat4 ViewProjectionMatrix;
};




void main()
{
	gl_Position = ViewProjectionMatrix * instanceModel * vec4(position,1.0);
}