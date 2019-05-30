#version 330 core

in vec4 color0;

out vec4 FragColor;

void main()
{

	FragColor = vec4(color0.x,color0.y,color0.z,color0.w);			
	
}