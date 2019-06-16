#version 330 core
#define MAX_PARTICLES 10000

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 offset;


out vec3 color0;

uniform mat4 transform[MAX_PARTICLES];

void main()
{
	gl_Position = transform[gl_InstanceID] * vec4(position + offset, 1.0);	
	color0 = color;
}