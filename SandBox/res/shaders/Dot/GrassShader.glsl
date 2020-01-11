#type vertex
#version 430 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 4) in vec2 a_TexCoord;
layout(location = 5) in mat4 a_InstanceModel;

layout(std140, binding = 0) uniform o_CameraData
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec3 ViewPos;
};

uniform float u_Time;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_ViewPos;


void main()
{
	vec3 pos = a_Position;
	
	vec4 WorldPos = a_InstanceModel * vec4(pos, 1.0);
	vec4 Position = ViewProjectionMatrix * WorldPos;
	
	gl_Position = Position;

	v_FragPos = Position.xyz;
	v_Normal = (ViewProjectionMatrix * a_InstanceModel * vec4(vec3(0,1,0), 0.0)).xyz;
	v_TexCoord = a_TexCoord;
	v_ViewPos = ViewPos;
}


#type fragment
#version 430 core

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_ViewPos;

layout(std140, binding = 1) uniform o_Light
{
	vec4 LightPosition;
	vec4 LightColor;
	float LightStrength;
};

uniform sampler2D u_Texture;

out vec4 color;

float c_AmbientStrength = 0.1;
float c_SpecularStrength = 0.5;

void main()
{
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(LightPosition.xyz - v_FragPos);

	vec3 ambient = c_AmbientStrength * LightColor.xyz;
	vec3 viewDir = normalize(v_ViewPos - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	float diff = max(dot(norm, lightDir), 0.0);

	vec3 specular = c_SpecularStrength * spec * LightColor.xyz;
	vec3 diffuse = diff * LightColor.xyz;
	vec3 result = (ambient + diffuse + specular) * LightStrength;

	vec4 texColor = texture(u_Texture, v_TexCoord) * vec4(result, 1.0);
	if (texColor.a < 0.1)
		discard;
	color = texColor;
}

