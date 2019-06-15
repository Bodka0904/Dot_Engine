#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 offset;

out vec3 color0;
flat out int InstanceID;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(position + offset, 1.0);	
	color0 = color;
	InstanceID = gl_InstanceID;
}