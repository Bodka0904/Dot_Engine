#include "stdafx.h"
#include "UniformBuffer.h"
#include <GL/glew.h>

namespace Dot {
	UniformBuffer::UniformBuffer(const void* data, unsigned int size, unsigned int index)
		:m_Index(index)
	{
		glGenBuffers(1, &m_UBO);
		glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
		glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);

		glBindBufferBase(GL_UNIFORM_BUFFER, index, m_UBO);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

	}

	UniformBuffer::~UniformBuffer()
	{
		glDeleteBuffers(1, &m_UBO);
	}


	void UniformBuffer::Update(const void* data, unsigned int size)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
		glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

}