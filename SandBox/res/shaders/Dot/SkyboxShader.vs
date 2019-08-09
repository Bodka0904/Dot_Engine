#version 330 core

layout (location = 0) in vec3 position;

out vec3 TexCoords;

layout (std140) uniform camera_data
{
	mat4 ViewProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};


void main()
{
	mat4 view = ViewMatrix;
	// set translation to zero, skybox will not move
	view[3][0] = 0;
	view[3][1] = 0;
	view[3][2] = 0;

    TexCoords = position;
    gl_Position = ProjectionMatrix * view * vec4(position, 1.0);
} 