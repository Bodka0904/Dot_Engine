#type vertex
#version 430 core

layout(location = 0) in vec3 a_Position;

layout(std140, binding = 0) uniform o_CameraData
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
#version 430 core


out vec4 FragColor;


void main()
{
	FragColor = vec4(1,1,1,1);
}