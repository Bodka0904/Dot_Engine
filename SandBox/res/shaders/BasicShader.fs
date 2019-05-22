#version 400 core

in vec2 texCoord0;
in vec3 normal0;
out vec4 FragColor;

float lightStrength;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 lightDir;

uniform sampler2D Texture;


void main()
{
	vec3 norm = normalize(normal0);
	vec3 ambient = lightStrength * lightColor;
	float diff = max(dot(norm,normalize(lightDir)), 0.0);
	
	vec3 diffuse = diff * lightColor;
	vec3 result = ambient + diffuse;


	FragColor = texture2D(Texture,texCoord0) 
			* vec4(result,1.0);
	
}