#pragma once
#include "Dot/Renderer/Buffers/GBuffer.h"

namespace Dot {
	class OpenGLGBuffer : public GBuffer
	{
	public:
		OpenGLGBuffer(uint32_t width, uint32_t height);

		virtual void Bind() const;
		virtual void Unbind() const;
		virtual void BindTexture(unsigned int unit, unsigned int index);
		virtual void Blit();
	private:
		enum
		{
			POSITION,
			NORMAL,
			NUM_ATTACHMENTS
		};

		unsigned int m_GBuffer;
		unsigned int m_DepthBuffer;
		unsigned int m_TextureAttachment[NUM_ATTACHMENTS];

		uint32_t m_Width; 
		uint32_t m_Height;
	};
}