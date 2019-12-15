#pragma once
#include "Buffer.h"


namespace Dot {

	class ArrayBuffer
	{
	public:
		~ArrayBuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
	
		virtual void AddVBO(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void AddIBO(const Ref<IndexBuffer>& indexBuffer) = 0;
		virtual void AddSSBO(const Ref<ShaderStorageBuffer>& shaderSBuffer) = 0;


		virtual Ref<VertexBuffer> GetVertexBuffer(int index) const = 0;
		virtual Ref<IndexBuffer> GetIndexBuffer() const = 0;

		static Ref<ArrayBuffer> Create();
	private:
		unsigned int m_VAO;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		std::vector<Ref<ShaderStorageBuffer>> m_ShaderBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};

}
