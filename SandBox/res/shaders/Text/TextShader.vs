#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 ortho;
out vec2 UV;

void main()
{

	gl_Position = ortho * vec4(position ,0,1);
	
	UV = texCoord;
}
