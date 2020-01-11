#include "stdafx.h"
#include "OpenGLUniformBuffer.h"

#include <GL/glew.h>

namespace Dot {
	OpenGLUniformBuffer::OpenGLUniformBuffer(const void* data, unsigned int size, unsigned int index)
		: m_Index(index)
	{
		glGenBuffers(1, &m_UBO);
		glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
		glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, index, m_UBO);
		
	}
	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_UBO);
	}
	void OpenGLUniformBuffer::Update(const void* data, unsigned int size, int offset)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	}
}