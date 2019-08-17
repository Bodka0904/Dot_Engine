#include "stdafx.h"
#include "ArrayBuffer.h"
#include <GL/glew.h>
#include "Dot/Debug/OpenGL/OpenGLErr.h"

namespace Dot {

	ArrayBuffer::ArrayBuffer()
	{
		
		glGenVertexArrays(1, &m_VAO);
	}

	ArrayBuffer::~ArrayBuffer()
	{
		glDeleteVertexArrays(1, &m_VAO);
	}

	void ArrayBuffer::Bind() const
	{
		glBindVertexArray(m_VAO);
	}

	void ArrayBuffer::UnBind() const
	{
		glBindVertexArray(0);
	}

	void ArrayBuffer::AddVBO(const Ref<VertexBuffer>& vertexBuffer)
	{
		D_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout");

		glBindVertexArray(m_VAO);
		vertexBuffer->Bind();

		
		const auto& layout = vertexBuffer->GetLayout();

		for (const auto& element : vertexBuffer->GetLayout())
		{
			glEnableVertexAttribArray(element.index);
			glVertexAttribPointer(element.index,
				element.GetComponentCount(),
				GL_FLOAT,
				GL_FALSE,
				layout.GetStride(),
				(const void*)element.offset);

			glVertexAttribDivisor(element.index,element.divisor);
	
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void ArrayBuffer::AddIBO(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_VAO);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}