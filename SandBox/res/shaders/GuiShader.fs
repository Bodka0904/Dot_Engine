#version 330 core

in vec4 color0;
uniform vec3 color_u;

out vec4 FragColor;

void main()
{	
	FragColor = vec4(color0.xyz + color_u,color0.w);	
}