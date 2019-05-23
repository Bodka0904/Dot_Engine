#version 330 core

out gl_FragColor;

in vec2 texCoord0;
in vec3 normal0;

float lightStrength
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 lightDir;

uniform vec3 uColor;

void main()
{
	vec3 ambient = lightStrength * lightColor;
	ver3 diffuse = max(dot(norm,normalize(lightDir)), 0.0);

	vec3 result = (ambient + difuse) * uColor;


	gl_FragColor = vec4(result, 1.0);
	
}