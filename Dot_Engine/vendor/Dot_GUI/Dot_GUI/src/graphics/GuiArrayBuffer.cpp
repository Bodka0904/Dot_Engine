#include "GuiArrayBuffer.h"
#include <GL/glew.h>



	GuiArrayBuffer::GuiArrayBuffer()
	{

		glGenVertexArrays(1, &m_VAO);
	}

	GuiArrayBuffer::~GuiArrayBuffer()
	{
		glDeleteVertexArrays(1, &m_VAO);
	}

	void GuiArrayBuffer::Bind() const
	{
		glBindVertexArray(m_VAO);
	}

	void GuiArrayBuffer::UnBind() const
	{
		glBindVertexArray(0);
	}

	void GuiArrayBuffer::AddVBO(const std::shared_ptr<GuiVertexBuffer>& vertexBuffer)
	{

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

			glVertexAttribDivisor(element.index, element.divisor);

		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void GuiArrayBuffer::AddIBO(const std::shared_ptr<GuiIndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_VAO);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
