#type vertex
#version 430 core
#define PI 3.1415926535897932384626433832795

layout(location = 0) in vec3 a_Position;



layout(std140, binding = 0) uniform o_CameraData
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec3 ViewPos;
};

uniform float u_Time;

const float c_WaveLength = 5;
const float c_WaveAmplitude = 2;
const vec3 RandSeed = vec3(12.9898, 78.233, 43758.5453);


float rand(vec2 co) 
{
	return fract(sin(dot(co.xy, vec2(RandSeed.x, RandSeed.y))) * RandSeed.z);
}

float GenerateOffset(float x, float z)
{
	float radiansX = (mod(x + z * rand(vec2(x,z)), c_WaveLength) / c_WaveLength + u_Time) * 2.0 * PI;
	float radiansZ = (mod(x * z + x * rand(vec2(x+z,z)), c_WaveLength) / c_WaveLength + u_Time) * 2.0 * PI;

	return c_WaveAmplitude * 0.5 * (cos(radiansX) + sin(radiansZ));
}

vec3 ApplyOffset(vec3 position)
{
	float xDisortion = GenerateOffset(position.x, position.z);
	float yDisortion = GenerateOffset(position.x, position.z);
	float zDisortion = GenerateOffset(position.x, position.z);

	return position + vec3(xDisortion, yDisortion, zDisortion);
}


out vec3 v_FragPos;
out vec3 v_ViewPos;
out vec3 v_Normal;

void main()
{
	vec3 newPosition = ApplyOffset(a_Position);
	vec4 Position = ViewProjectionMatrix * vec4(newPosition, 1.0);

	gl_Position = Position;

	v_FragPos = newPosition;
	v_ViewPos = ViewPos;
	v_Normal = vec3(newPosition.z-a_Position.z, newPosition.y, newPosition.x-a_Position.x);
}


#type fragment
#version 430 core


in vec3 v_FragPos;
in vec3 v_ViewPos;
in vec3 v_Normal;

layout(std140, binding = 1) uniform o_Light
{
	vec4 LightPosition;
	vec4 LightColor;
	float LightStrength;
};


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

	color = vec4(0,0.3,0.7,0.5) * vec4(result,1.0);
}
