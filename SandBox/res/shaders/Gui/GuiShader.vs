#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec2 transformation;


uniform mat4 mvp_u;
uniform vec2 pos_u;

out vec2 tex0;


void main()
{
	gl_Position =  mvp_u * vec4(position) + transformation,0,1.0);
	
	tex0 = texCoords;
}