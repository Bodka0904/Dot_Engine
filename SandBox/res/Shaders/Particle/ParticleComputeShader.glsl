#type compute
#version 430
//Size of compute shader local work group - x=32, y=32, z=1(default)
layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

const int c_MaxParticles = 10000;


struct ParticleAttributeData
{
	vec4  velocity;
	int changeSize;
	float lifeSpan;
	float gravity;
	float unused;
};

layout(std430,binding = 0) buffer
buffer_InPos
{
	vec4 InPos[];
};

layout(std430,binding = 1) buffer
buffer_InAttribute
{
	ParticleAttributeData InAttribute[];
};

layout(std430,binding = 2) buffer
buffer_OutPos
{
	vec4 OutPos[];
};

layout(std430,binding = 3) buffer
buffer_OutAttribute
{
	ParticleAttributeData OutAttribute[];
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
	vec4 particleVel = InAttribute[index].velocity;

	float particleLifeSpan = InAttribute[index].lifeSpan;
	float particleGravity = InAttribute[index].gravity;
	int particleChangeSize = InAttribute[index].changeSize;

	
	particlePos.x += (particleVel.x / 600) * u_Speed ;
	particlePos.y += (particleVel.y / 600) * u_Speed ;
	particlePos.z += (particleVel.z / 600) * u_Speed ;

	
	particleVel.w += u_Time;
		
	if (particleChangeSize > 1)
		particlePos.w = 1;
	else
		particlePos.w = particleChangeSize - (particleVel.w / particleLifeSpan);

	if (particleVel.w > particleLifeSpan)
	{
		particleVel.w = 0;
		if (particleChangeSize > 1)
			particlePos = vec4(0, 0, 0, 1);
		else
			particlePos = vec4(0, 0, 0, particleChangeSize);
		
	}

	OutPos[index] = particlePos;
	OutAttribute[index].velocity = particleVel;
}