#include "stdafx.h"
#include "OpenGLFrameBuffer.h"
#include "Dot/Core/Input.h"

#include <GL/glew.h>

namespace Dot {
	OpenGLFramebuffer::OpenGLFramebuffer(uint32_t width, uint32_t height, FramebufferFormat format)
		: m_Format(format)
	{
		Resize(width, height);
		m_Width = width;
		m_Height = height;
	}
	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_FBO);
		glDeleteTextures(1, &m_DepthAttachment);
		glDeleteTextures(1, &m_ColorAttachment);
	}
	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (m_Width == width && m_Height == height)
		{
			return;
		}
		m_Width = width;
		m_Height = height;

		if (m_FBO)
		{
			glDeleteFramebuffers(1, &m_FBO);
			glDeleteTextures(1, &m_ColorAttachment);
			glDeleteTextures(1, &m_DepthAttachment);
		}

		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		glGenTextures(1, &m_ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
		if (m_Format == FramebufferFormat::RGB)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height,
				0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		}
		else if (m_Format == FramebufferFormat::RGBA16F)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, nullptr);
		}
		else if (m_Format == FramebufferFormat::RGBA8)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_ColorAttachment, 0);

		glGenTextures(1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthAttachment, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			LOG_ERR("Framebuffer is incomplete!");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFramebuffer::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glViewport(0, 0, m_Width, m_Height);
	}
	void OpenGLFramebuffer::Unbind() const
	{
		auto WinSize = Input::GetWindowSize();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WinSize.first, WinSize.second);
	}
	void OpenGLFramebuffer::BindTexture(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
	}
}