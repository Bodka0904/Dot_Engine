#type compute
#version 430
//Size of compute shader local work group - x=32, y=32, z=1(default)
layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

const int c_MaxParticles = 10000;


struct ParticleData
{
	vec4  velocity;
	float lifeSpan;
	float gravity;
	float unUsed;
	float unUsed2;
};

layout(std430, binding = 0) buffer
buffer_InPos
{
	vec4 InPos[];
};

layout(std430, binding = 1) buffer
buffer_InData
{
	ParticleData InData[];
};

uniform float u_Time;
uniform float u_Speed;

float rand(vec2 co)
{
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453) * 1000.0;
}



void main(void)
{
	uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;

	if (index > c_MaxParticles)
		return;

	vec4 particlePos = InPos[index];
	vec4 particleVel = InData[index].velocity;

	float particleLifeSpan = InData[index].lifeSpan;

	particlePos.x += (particleVel.x / 600) * u_Speed;
	particlePos.y += (particleVel.y / 600) * u_Speed;
	particlePos.z += (particleVel.z / 600) * u_Speed;


	particleVel.w += u_Time;

	if (particleVel.w > particleLifeSpan)
	{
		particleVel.w = 0;
		particlePos = vec4(0, 0, 0, 1);
	}

	InPos[index] = particlePos;
	InData[index].velocity = particleVel;
}