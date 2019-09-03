#version 430

//Size of compute shader local work group - x=32, y=32, z=1(default)
layout( local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout ( binding = 0 ) buffer
buffer_InPos
{
	vec4	InPos[];
};

layout ( binding = 1 ) buffer
buffer_InVelocity
{
	vec4	InVelocity[];
};

layout ( binding = 2 ) buffer
buffer_OutPos
{
	vec4	OutPos[];
};

layout ( binding = 3 ) buffer
buffer_OutVelocity
{
	vec4	OutVelocity[];
};


const int g_NumParticles = 1000;

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453) * 1000.0;
}

void main(void)
{
	uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;

	if(index > g_NumParticles)
		return;

	vec4 particlePos = InPos[index];
	
	vec4 particleVelocity = InVelocity[index];
	vec4 newParticleVelocity = particleVelocity;

	newParticleVelocity.xyz *= 1.02;
	newParticleVelocity.w -= 0.05;

	particlePos.x += particleVelocity.x/500;
	particlePos.y += particleVelocity.y/600;
	particlePos.z += particleVelocity.z/400;

	if (newParticleVelocity.w < 0)
	{
		float rand1 = rand(particlePos.xz);
		float rand2 = rand(particlePos.zx);

		particlePos.xyz = vec3(0,0,10);

		newParticleVelocity.x = -5.0 + mod(rand2 *100.0f, 10.0);
		newParticleVelocity.y =  5.0 + mod(rand2 *100.0f, 10.0);
		newParticleVelocity.z = -5.0 + mod(rand1 *100.0f, 10.0);

		newParticleVelocity.w = rand1/100;
	}

	OutPos[index] = particlePos;
	OutVelocity[index] = newParticleVelocity;

	
}