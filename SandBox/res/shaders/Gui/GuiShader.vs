#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;


uniform mat4 mvp_u;
uniform vec2 pos_u;
uniform vec2 scale_u;

out vec2 tex0;


void main()
{
	gl_Position =  mvp_u * vec4((scale_u * position) + pos_u,0,1.0);
	
	tex0 = texCoords;
}