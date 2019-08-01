#pragma once
#include "GuiBuffer.h"
#include <memory>
#include <vector>



	class GuiArrayBuffer
	{
	public:
		GuiArrayBuffer();
		~GuiArrayBuffer();

		void Bind() const;
		void UnBind() const;

		void AddVBO(const std::shared_ptr<GuiVertexBuffer>& vertexBuffer);
		void AddIBO(const std::shared_ptr<GuiIndexBuffer>& indexBuffer);

		std::shared_ptr<GuiVertexBuffer> GetVertexBuffer(int index) const { return m_VertexBuffers[index]; }
		std::shared_ptr<GuiIndexBuffer> GetIndexBuffer() const { return m_IndexBuffer; }

	private:
		unsigned int m_VAO;
		std::vector<std::shared_ptr<GuiVertexBuffer>> m_VertexBuffers;
		std::shared_ptr<GuiIndexBuffer> m_IndexBuffer;
	};


