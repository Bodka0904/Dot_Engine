#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out VS_OUT {
    vec2 texCoords;
	vec3 normals;
} vs_out;

layout (std140) uniform camera_data
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

uniform mat4 ModelMatrix;

out vec3 fragPos0;

void main()
{
	vs_out.texCoords = texCoord;
	vs_out.normals = (ViewProjectionMatrix * ModelMatrix * vec4(normal, 0.0)).xyz;

	gl_Position = ViewProjectionMatrix * ModelMatrix * vec4(position,1.0);	
		
	fragPos0 = vec3(ViewProjectionMatrix * ModelMatrix * vec4(position,1.0));	

}