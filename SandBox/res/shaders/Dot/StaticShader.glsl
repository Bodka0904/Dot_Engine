#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Binormal;
layout(location = 4) in vec2 a_TexCoord;


layout(std140) uniform camera_data
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec3 ViewPos;
};

uniform vec2 u_ClipDistance;
uniform mat4 u_ModelMatrix;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_ViewPos;


void main()
{
	vec4 plane = vec4(0, u_ClipDistance.x, 0, u_ClipDistance.y);
	vec4 WorldPos = u_ModelMatrix * vec4(a_Position, 1.0);

	gl_ClipDistance[0] = dot(WorldPos, plane);
	vec4 Position = ViewProjectionMatrix * WorldPos;

	gl_Position = Position;

	v_FragPos = Position.xyz;
	v_Normal = (ViewProjectionMatrix * u_ModelMatrix * vec4(a_Normal, 0.0)).xyz;
	v_TexCoord = a_TexCoord;
	v_ViewPos = ViewPos;
}



#type fragment
#version 330 core

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_ViewPos;

uniform vec3 u_LightPosition;
uniform vec3 u_LightColor;
uniform float u_LightStrength;


uniform sampler2D u_Texture;

out vec4 color;

void main()
{
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(u_LightPosition - v_FragPos);
	
	vec3 ambient = u_LightStrength * u_LightColor;
	vec3 viewDir = normalize(v_ViewPos - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	float diff = max(dot(norm, lightDir), 0.0);

	vec3 specular = spec * u_LightColor * 0.2;
	vec3 diffuse = diff * u_LightColor;
	vec3 result = (ambient + diffuse + specular);

	vec4 texColor = texture(u_Texture, v_TexCoord) * vec4(result, 1.0);
	if (texColor.a < 0.1)
		discard;
	color = texColor;

}