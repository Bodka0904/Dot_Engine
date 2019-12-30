#type compute
#version 430
//Size of compute shader local work group - x=32, y=32, z=1(default)
layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

const int c_MaxParticles = 1000;



layout(binding = 0) buffer
buffer_InPos
{
	vec4 InPos[];
};

layout(binding = 1) buffer
buffer_InAttribute
{
	vec4 InAttribute[];
};

layout(binding = 2) buffer
buffer_OutPos
{
	VertexData OutPos[];
};

layout(binding = 3) buffer
buffer_OutAttribute
{
	vec4 OutAttribute[];
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

	vec4 particlePos = InPos[index].pos;
	vec4 particleVel = InAttribute[index];
		

	particlePos.x += (particleVel.x/600) * u_Speed;
	particlePos.y += (particleVel.y/600) * u_Speed;
	particlePos.z += (particleVel.z/600) * u_Speed;

	particlePos.w -= u_Time;
	
	if (particlePos.w < 0)
	{
		particlePos.w = particleVel.w; 
		particlePos.xyz = vec3(0, 0, 10);
	}

	OutPos[index] = particlePos;
}