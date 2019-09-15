#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 4) in vec2 a_TexCoord;
layout(location = 5) in mat4 a_InstanceModel;

layout(std140) uniform camera_data
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_TexCoord;


void main()
{
	gl_Position = ViewProjectionMatrix * a_InstanceModel * vec4(a_Position, 1.0);

	v_FragPos = vec3(ViewProjectionMatrix * a_InstanceModel * vec4(a_Position, 1.0));
	v_Normal = (ViewProjectionMatrix * a_InstanceModel * vec4(a_Normal, 0.0)).xyz;
	v_TexCoord = a_TexCoord;
}


#type fragment
#version 330 core

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;


uniform float lightStrength;
uniform float specStrength;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 lightPos;

uniform sampler2D u_Texture;

out vec4 color;

void main()
{
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(vec3(0, 5, 0) - v_FragPos);
	vec3 ambient = 0.7 * vec3(0.5, 0.5, 0.5);
	vec3 viewDir = normalize(vec3(0, 5, 0) - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	float diff = max(dot(norm, vec3(0, 5, 0)), 0.0);

	vec3 specular = 0.7 * spec * vec3(0.5, 0.5, 0.5);
	vec3 diffuse = diff * vec3(0.5, 0.5, 0.5);
	vec3 result = ambient + diffuse + specular;

	color = texture(u_Texture, v_TexCoord) * vec4(result, 1.0);
}

