#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 offset;


out vec2 texCoord0;
out vec3 normal0;


void main()
{
	gl_Position = vec4(position + offset, 1.0);	

	texCoord0 = texCoord;
	normal0 = (transform * vec4(normal, 0.0)).xyz;
}