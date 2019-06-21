#version 330 core

in vec2 tex0;

uniform vec3 color_u;


out vec4 FragColor;
uniform sampler2D tex;


void main()
{	
	FragColor = texture(tex,tex0) * vec4(1 - color_u.x,1 - color_u.y,1 - color_u.z,0.5);	
}