#type vertex
#version 330

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

layout(std140) uniform camera_data
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec3 ViewPos;
};
uniform mat4 u_ModelMatrix;

out vec2 v_TexCoord;

void main()
{
	vec4 Position = ViewProjectionMatrix * vec4(a_Position, 1.0);

	gl_Position = Position;

	v_TexCoord = a_TexCoord;

}


#type fragment
#version 330 core


in vec2 v_TexCoord;



uniform sampler2D u_Texture;

out vec4 color;

void main()
{
	color = texture(u_Texture, v_TexCoord) * vec4(1,1,1,0.3);

}