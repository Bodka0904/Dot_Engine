#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec4 color;

uniform mat4 mvp_u;


out vec4 color0;


void main()
{
	gl_Position = mvp_u * vec4(position,0,1.0);
	
	color0 = color;
}