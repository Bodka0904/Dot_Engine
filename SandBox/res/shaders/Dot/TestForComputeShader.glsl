#type vertex
#version 330 core

layout(location = 0) in vec4 a_Position;

layout(std140) uniform camera_data
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

uniform mat4 u_ModelMatrix;



void main()
{
	gl_Position = ViewProjectionMatrix * vec4(a_Position.xyz, 1.0);

}


#type fragment
#version 330 core


out vec4 color;

void main()
{
	color = vec4(1, 0, 0, 1);
}
