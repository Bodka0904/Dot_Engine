#include "stdafx.h"
#include "UniformBuffer.h"
#include <GL/glew.h>

Dot::UniformBuffer::UniformBuffer(const void*data, unsigned int size, unsigned int index)
	:m_Index(index),m_size(size)
{
	glGenBuffers(1, &m_UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
	glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, index, m_UBO);	
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

Dot::UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &m_UBO);
}


void Dot::UniformBuffer::Update(const void* data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
	GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	memcpy(p, data, m_size);
	glUnmapBuffer(GL_UNIFORM_BUFFER);	
}
