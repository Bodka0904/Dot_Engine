#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 binormal;
layout (location = 4) in vec2 texCoord;


layout (std140) uniform camera_data
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

uniform mat4 ModelMatrix;

out vec3 fragPos0;
out vec3 normal0;
out vec2 texcoord0;

void main()
{
	gl_Position = ViewProjectionMatrix * ModelMatrix * vec4(position,1.0);	
		
	fragPos0 = vec3(ViewProjectionMatrix * ModelMatrix * vec4(position,1.0));	
	normal0 = (ViewProjectionMatrix * ModelMatrix * vec4(normal, 0.0)).xyz;
	texcoord0 = texCoord;
}