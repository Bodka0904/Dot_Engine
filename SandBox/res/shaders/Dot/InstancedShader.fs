#version 330 core

in vec3 fragPos0;
in vec2 texcoord0;
in vec3 normal0;


uniform sampler2D tex;

out vec4 color;

void main()
{
	color = texture(tex,texcoord0);
}
