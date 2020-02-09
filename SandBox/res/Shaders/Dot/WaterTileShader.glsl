#type vertex
#version 430 core
#define PI 3.1415926535897932384626433832795

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec4 v_ClipSpace;
out vec3 v_WorldPos;
out vec3 v_CameraPos;
out vec2 v_TexCoord;
out float v_Time;

layout(std140, binding = 0) uniform o_CameraData
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec3 ViewPos;
};

uniform float u_Time;

void main()
{
	v_ClipSpace = ViewProjectionMatrix * vec4(a_Position, 1.0); // later add model matrix
	gl_Position = v_ClipSpace;
	v_TexCoord = a_TexCoord;
	v_WorldPos = a_Position; // Later add model matrix
	v_CameraPos = ViewPos; 
	v_Time = u_Time;
}


#type fragment
#version 430 core

in vec4 v_ClipSpace;
in vec3 v_WorldPos;
in vec3 v_CameraPos;
in vec2 v_TexCoord;
in float v_Time;

layout(std140, binding = 1) uniform o_Light
{
	vec4 LightPosition;
	vec4 LightColor;
	float LightStrength;
};

const float c_DistortionStrength = 0.03;
const float c_WaveSpeed = 0.01;
const float c_ShineDamper = 20;
const float c_Reflectivity = 0.6;



uniform sampler2D normalMap;
uniform sampler2D reflectTexture;
uniform sampler2D dudvMapTexture;

out vec4 color;

void main()
{
	vec3 cameraVector = v_CameraPos - v_WorldPos;
	vec3 lightVector = v_WorldPos - LightPosition.xyz;

	float moveFactor = v_Time * c_WaveSpeed;
	vec2 distortionCoord = texture(dudvMapTexture, vec2(v_TexCoord.x + moveFactor, v_TexCoord.y)).xy * 0.1;
	distortionCoord = v_TexCoord + vec2(distortionCoord.x, distortionCoord.y + moveFactor);
	vec2 distortion = (texture(dudvMapTexture,distortionCoord) * 2.0 - 1.0).xy * c_DistortionStrength;

	vec2 ndc = (v_ClipSpace.xy / v_ClipSpace.w) / 2.0 + 0.5;
	vec2 reflectTexCoord = vec2(ndc.x, -ndc.y);

	reflectTexCoord += distortion.xy;
	reflectTexCoord.x = clamp(reflectTexCoord.x, 0.001, 0.999);
	reflectTexCoord.y = clamp(reflectTexCoord.y, -0.999, -0.001);

	vec3 viewVector = normalize(cameraVector);
	float refractiveFactor = dot(viewVector, vec3(0.0, 1.0, 0.0));
	refractiveFactor = pow(refractiveFactor, 0.5);

	vec4 normalMap = texture(normalMap, distortionCoord);
	vec3 normal = vec3(normalMap.r * 2.0 - 1.0, normalMap.b, normalMap.g * 2.0 - 1.0);
	normal = normalize(normal);

	vec3 reflectedLight = reflect(normalize(lightVector), normal);
	float specular = max(dot(reflectedLight, viewVector), 0.0);
	specular = pow(specular, c_ShineDamper);
	vec3 specularHighlights = LightColor.xyz * specular * c_Reflectivity;

	vec4 reflectColor = texture(reflectTexture, reflectTexCoord);
	color = mix(reflectColor,vec4(0,0.3,0.5,0.5) + vec4(specularHighlights, 0.0),refractiveFactor);
}
