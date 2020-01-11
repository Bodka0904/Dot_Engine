#type vertex
#version 430 core

layout(location = 0) in vec3 a_Position;

out vec3 v_TexCoord;

layout(std140, binding = 0) uniform o_CameraData
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec3 ViewPos;
};


void main()
{
	mat4 view = ViewMatrix;
	// set translation to zero, skybox will not move
	view[3][0] = 0;
	view[3][1] = 0;
	view[3][2] = 0;

	v_TexCoord = a_Position;
	gl_Position = ProjectionMatrix * view * vec4(a_Position, 1.0);
}

#type fragment
#version 430 core

out vec4 FragColor;
in vec3 v_TexCoord;

uniform samplerCube u_Texture;

void main()
{
	FragColor = texture(u_Texture, v_TexCoord);
}