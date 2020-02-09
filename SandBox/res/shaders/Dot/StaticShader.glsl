#type vertex
#version 430 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Binormal;
layout(location = 4) in vec2 a_TexCoord;


layout(std140, binding = 0) uniform o_CameraData
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec3 ViewPos;
};

uniform mat4 u_ModelMatrix;

out vec2 v_TexCoord;
out vec3 v_SurfaceNormal;
out vec3 v_CameraVector;
out vec4 v_WorldPos;

void main()
{
	vec4 WorldPos = u_ModelMatrix * vec4(a_Position, 1.0);
	gl_Position = ViewProjectionMatrix * WorldPos;

	v_TexCoord = a_TexCoord;
	v_SurfaceNormal = vec4(u_ModelMatrix * vec4(a_Normal, 0.0)).xyz;
	v_WorldPos = WorldPos;
	v_CameraVector = vec4(inverse(ViewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - WorldPos.xyz;
}


#type fragment
#version 430 core

in vec2 v_TexCoord;
in vec3 v_SurfaceNormal;
in vec3 v_CameraVector;
in vec4 v_WorldPos;

layout(std140, binding = 1) uniform o_Light
{
	vec4 LightPosition;
	vec4 LightColor;
	float LightStrength;
};
uniform float u_Shininess;
uniform float u_Reflectivity;

uniform float u_Radius;
uniform vec2 u_BrushPosition;

uniform sampler2D u_Texture;
const vec3 c_BrushColor = vec3(0.2, 0.7, 1.0);

out vec4 color;

vec4 CalculateLight()
{
	// Diffuse
	vec3 lightVector = LightPosition.xyz - v_WorldPos.xyz;
	vec3 unitNormal = normalize(v_SurfaceNormal);
	vec3 unitLightVector = normalize(lightVector);

	float result = dot(unitNormal, unitLightVector);
	float brightness = max(result, 0.0);
	vec3 diffuse = brightness * LightColor.xyz;

	// Specular
	vec3 unitCameraVector = normalize(v_CameraVector);
	vec3 lightDirection = -unitLightVector;
	vec3 reflectLightDirection = reflect(lightDirection,unitNormal);
	
	float specFactor = dot(reflectLightDirection, unitCameraVector);
	specFactor = max(specFactor, 0.0);
	float dampFactor = pow(specFactor, u_Shininess);
	vec3 specular = dampFactor * u_Reflectivity * LightColor.xyz;

	return vec4(diffuse + specular, 1.0);
}

void main()
{
	float dist = length(u_BrushPosition - v_WorldPos.xz);
	vec3 result = CalculateLight().xyz;
	if (dist < u_Radius)
		result += c_BrushColor;

	color = texture(u_Texture, v_TexCoord) * vec4(result,1.0);
	
}