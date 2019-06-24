#version 330 core

in vec3 fragPos0;
in vec2 texCoord0;
in vec3 normal0;

uniform float lightStrength;
uniform float specStrength;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 lightPos;

uniform sampler2D tex;

out vec4 color;

void main()
{
	vec3 norm = normalize(normal0);
	vec3 lightDir = normalize(lightPos - fragPos0);
	vec3 ambient = lightStrength * lightColor;
	vec3 viewDir = normalize(viewPos - fragPos0);
	vec3 reflectDir = reflect(-lightDir,norm);

	float spec = pow(max(dot(viewDir,reflectDir),0.0), 32);
	float diff = max(dot(norm,lightDir), 0.0);
	
	vec3 specular = specStrength * spec * lightColor;
	vec3 diffuse = diff * lightColor;
	vec3 result = ambient + diffuse + specular;

	color = texture(tex,texCoord0) * vec4(result,1.0);			
	
}