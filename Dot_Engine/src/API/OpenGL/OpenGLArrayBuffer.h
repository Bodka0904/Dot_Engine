#pragma once
#include "Dot/Renderer/Buffers/ArrayBuffer.h"

namespace Dot {
	class OpenGLArrayBuffer : public ArrayBuffer
	{
	public:
		OpenGLArrayBuffer();
		~OpenGLArrayBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void AddVBO(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void AddIBO(const Ref<IndexBuffer>& indexBuffer) override;
		virtual void AddSSBO(const Ref<ShaderStorageBuffer>& shaderSBuffer) override;


		virtual Ref<VertexBuffer> GetVertexBuffer(int index) const override { return m_VertexBuffers[index]; };
		virtual Ref<IndexBuffer> GetIndexBuffer() const override { return m_IndexBuffer; }

	private:
		unsigned int m_VAO;
		Ref<IndexBuffer> m_IndexBuffer;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		std::vector<Ref<ShaderStorageBuffer>> m_ShaderBuffers;
	};
}