#version 330 core

out vec4 m_color;

in vec3 color0;

void main()
{
	m_color = vec4(m_color);
}