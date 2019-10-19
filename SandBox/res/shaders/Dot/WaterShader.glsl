#type vertex
#version 330
#define PI 3.1415926535897932384626433832795

layout(location = 0) in vec3 a_Position;



layout(std140) uniform camera_data
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec3 ViewPos;
};
uniform mat4 u_ModelMatrix;
uniform float u_Time;

const float c_WaveLength = 3;
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
#version 330 core


in vec3 v_FragPos;
in vec3 v_ViewPos;
in vec3 v_Normal;

uniform vec3 u_LightPosition;
uniform vec3 u_LightColor;
uniform float u_LightStrength;


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

	vec3 specular = u_LightStrength * spec * u_LightColor;
	vec3 diffuse = diff * u_LightColor;
	vec3 result = (ambient + diffuse + specular);

	color = vec4(0,0.3,0.7,0.5) * vec4(result,1.0);
}
