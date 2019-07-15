#include "stdafx.h"
#include "Buffer.h"
#include "Dot/Graphics/ObjLoader.h"
#include <GL/glew.h>

namespace Dot {

	VertexBuffer::VertexBuffer(const void* vertices, unsigned int size, BufferFlag flag)
	{
		
		glCreateBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		if (flag & Static_Buffer_Update)
		{
			glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
		}
		else if (flag & Dynamic_Buffer_Update)
		{
			glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
		}
	}



	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_VBO);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	}

	void VertexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::Update(const void * vertices,unsigned int size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);		
	}



	IndexBuffer::IndexBuffer(unsigned int* indices, unsigned int count)
		:m_Count(count)
	{
		glCreateBuffers(1, &m_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_VBO);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	}

	void IndexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	unsigned int IndexBuffer::GetCount() const
	{
		return m_Count;
	}


}