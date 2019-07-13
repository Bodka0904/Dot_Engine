#version 330 core

in vec2 tex0;
in float x0;

uniform vec3 color_u;
uniform vec2 texOffset_u;
uniform float transparent_u;

out vec4 FragColor;
uniform sampler2D tex;


void main()
{	
	FragColor = texture(tex,vec2(tex0.x+texOffset_u.x,tex0.y+texOffset_u.y)) * vec4(1 - color_u.x,1 - color_u.y,1 - color_u.z,transparent_u);		
}