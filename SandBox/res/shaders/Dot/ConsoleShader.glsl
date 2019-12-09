#type vertex
#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjectionMatrix;


void main()
{
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 0, 1.0);
}


#type fragment
#version 330 core

in vec2 v_TexCoord;

out vec4 FragColor;


void main()
{	
	FragColor = vec4(0,0,0,0.9);
}
