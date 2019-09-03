#version 330 core

layout (location = 0) in vec4 position;

layout (std140) uniform camera_data
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

uniform mat4 ModelMatrix;



void main()
{
	gl_Position = ViewProjectionMatrix * vec4(position.xyz,1.0);	
		
}