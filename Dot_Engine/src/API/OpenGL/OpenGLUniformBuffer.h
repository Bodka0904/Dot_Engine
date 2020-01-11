#pragma once
#include "Dot/Renderer/Shader/UniformBuffer.h"

namespace Dot {

	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(const void* data, unsigned int size, unsigned int index);
		virtual ~OpenGLUniformBuffer();

		virtual void Update(const void* data, unsigned int size,int offset) override;
	private:
		unsigned int m_UBO;
		unsigned int m_Index;
	};

}