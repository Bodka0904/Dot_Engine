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

// layout( binding = 2, rgba32f) uniform image2D inVelocity;
// layout( binding = 3, rgba32f) uniform image2D outVelocity;


const int g_NumParticles = 100;



void main(void)
{
	uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;

	if(index > g_NumParticles)
		return;  

	InPos[index].x += 0.05;
	InPos[index].y += 0.05;
	InPos[index].z += 0.05;
	InPos[index].w  = 1;
}