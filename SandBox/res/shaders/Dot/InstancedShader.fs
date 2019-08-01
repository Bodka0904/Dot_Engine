#version 330 core

in vec3 fragPos0;
in vec2 texcoord0;
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
	vec3 lightDir = normalize(vec3(0,5,0) - fragPos0);
	vec3 ambient = 0.7 * vec3(0.5,0.5,0.5);
	vec3 viewDir = normalize(vec3(0,5,0) - fragPos0);
	vec3 reflectDir = reflect(-lightDir,norm);

	float spec = pow(max(dot(viewDir,reflectDir),0.0), 32);
	float diff = max(dot(norm,vec3(0,5,0)), 0.0);
	
	vec3 specular = 0.7 * spec * vec3(0.5,0.5,0.5);
	vec3 diffuse = diff * vec3(0.5,0.5,0.5);
	vec3 result = ambient + diffuse + specular;

	color = texture(tex,texcoord0) * vec4(result,1.0);	
}
