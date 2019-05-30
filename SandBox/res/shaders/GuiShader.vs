#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec4 color;

out vec4 color0;

uniform vec2 transform;

void main()
{
	gl_Position = vec4(position + transform,0.0,1.0);

	color0 = color;
		
}