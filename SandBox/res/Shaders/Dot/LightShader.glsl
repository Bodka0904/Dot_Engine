#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

layout(std140) uniform camera_data
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec3 ViewPos;
};

uniform mat4 u_ModelMatrix;

void main()
{
	gl_Position = ViewProjectionMatrix * u_ModelMatrix* vec4(a_Position, 1.0);
}

#type fragment
#version 330 core


out vec4 FragColor;


void main()
{
	FragColor = vec4(1,1,1,1);
}