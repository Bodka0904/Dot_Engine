#pragma once

#include "Dot/Renderer/Buffers/FrameBuffer.h"

namespace Dot {
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(uint32_t width, uint32_t height, FramebufferFormat format);
		~OpenGLFramebuffer();

		virtual void Resize(uint32_t width, uint32_t height);

		virtual void Bind() const ;
		virtual void Unbind() const;

		virtual void BindTexture(uint32_t slot = 0) const;

		virtual unsigned int GetFBO() const { return m_FBO; }
		virtual unsigned int GetColorAttachment() const { return m_ColorAttachment; }
		virtual unsigned int GetDepthAttachment() const { return m_DepthAttachment; }

		virtual uint32_t GetWidth() const { return m_Width; }
		virtual uint32_t GetHeight() const { return m_Height; }
		virtual FramebufferFormat GetFormat() const { return m_Format; }
	private:
		unsigned int m_FBO;

		uint32_t m_Width, m_Height;
		FramebufferFormat m_Format;

		unsigned int m_ColorAttachment, m_DepthAttachment;
	};

}