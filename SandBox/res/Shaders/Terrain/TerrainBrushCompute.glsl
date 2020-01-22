#type compute
#version 430
//Size of compute shader local work group - x=32, y=32, z=1(default)
layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout(std430, binding = 0) buffer
buffer_InPos
{
	vec4 InPos[];
};
layout(std430, binding = 1) buffer
buffer_InNormal
{
	vec4 InNormal[];
};


uniform vec2 u_BrushPosition;
uniform float u_Radius;
uniform float u_Strength;
uniform float u_SmoothFactor;
uniform bool u_Smooth;
uniform float u_EdgeSize;
uniform int u_Width;

bool IsInRadius(vec4 pos)
{
	if ((pos.x - u_BrushPosition.x) * (pos.x - u_BrushPosition.x) +
		(pos.z - u_BrushPosition.y) * (pos.z - u_BrushPosition.y) <= u_Radius * u_Radius)
		return true;
	else return false;
}

float SmoothPoint(uint index)
{
	float left = InPos[index - 1].y;
	float right = InPos[index + 1].y;
	float bottom = InPos[index - uint(u_Width - 1)].y;
	float top = InPos[index + uint(u_Width - 1)].y;

	return (left + right + bottom + top) / 4;
}

void main(void)
{
	uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;
	
	vec4 pos = InPos[index];
	vec4 normal = InNormal[index];

	if (IsInRadius(pos))
	{
		float heightL = InPos[index - 1].y;
		float heightR = InPos[index + 1].y;
		float heightD = InPos[index - 1].y;
		float heightU = InPos[index + 1].y;

		normal.xyz = vec3(heightD - heightU, 2.0f, heightL - heightR);
		InNormal[index].xyz = normalize(normal.xyz);

		float factor = distance(pos.xz, u_BrushPosition);
		
		pos.y += u_Strength - (u_SmoothFactor * ((factor/u_Radius) * u_Strength));

		if (u_Smooth)
		{
			pos.y = SmoothPoint(index);
		}
	}	
	
	InPos[index] = pos;
}