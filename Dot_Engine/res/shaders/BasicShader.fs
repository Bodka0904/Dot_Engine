#version 330 core

out gl_FragColor;

in vec2 texCoord0;
in vec3 normal0;



uniform sampler2D diffuse;
uniform vec3 uColor;

void main()
{
	
	gl_FragColor = texture2D(diffuse,texCoord0) 
			*clamp(dot(-vec3(0,0,1), normal0),0.0,1.0) * 4;
	
}