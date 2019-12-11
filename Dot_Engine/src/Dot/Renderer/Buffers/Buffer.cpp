#include "stdafx.h"
#include "Buffer.h"

#include <GL/glew.h>



namespace Dot {

	VertexBuffer::VertexBuffer(const void* vertices, unsigned int size,int drawMod)
		: m_Count(size/sizeof(float))
	{	
		glCreateBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, drawMod);
		
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

	unsigned int VertexBuffer::GetCount() const
	{
		return m_Count;
	}

	void VertexBuffer::Update(const void * vertices,unsigned int size,int offset)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);	
	}



	IndexBuffer::IndexBuffer(const void* indices, unsigned int count)
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


	ShaderStorageBuffer::ShaderStorageBuffer(const void* data, unsigned int size, int drawMod)
		:m_size(size)
	{
		glGenBuffers(1, &m_SSBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, drawMod);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	}

	ShaderStorageBuffer::~ShaderStorageBuffer()
	{
		glDeleteBuffers(1, &m_SSBO);
	}

	void ShaderStorageBuffer::BindBase(unsigned int point)
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, point, m_SSBO);
	}

	void ShaderStorageBuffer::BindRange(unsigned int index)
	{
		glBindBufferRange(GL_SHADER_STORAGE_BUFFER,index, m_SSBO,0, m_size);
	}

	void ShaderStorageBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_SSBO);
	}

}