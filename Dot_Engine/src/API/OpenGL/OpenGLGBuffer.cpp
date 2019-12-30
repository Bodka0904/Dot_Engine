#include "stdafx.h"
#include "OpenGLGBuffer.h"

#include <GL/glew.h>

namespace Dot {
	OpenGLGBuffer::OpenGLGBuffer(uint32_t width, uint32_t height)
		: m_Width(width),m_Height(height)
	{
		glGenFramebuffers(1, &m_GBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffer);


		glGenTextures(1, &m_TextureAttachment[POSITION]);
		glBindTexture(GL_TEXTURE_2D, m_TextureAttachment[POSITION]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureAttachment[POSITION], 0);


		glGenTextures(1, &m_TextureAttachment[NORMAL]);
		glBindTexture(GL_TEXTURE_2D, m_TextureAttachment[NORMAL]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_TextureAttachment[NORMAL], 0);

		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
		glDrawBuffers(2, attachments);


		glGenRenderbuffers(1, &m_DepthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);
		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			LOG_ERR("GBuffer not complete!");
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLGBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffer);
	}
	void OpenGLGBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLGBuffer::BindTexture(unsigned int unit, unsigned int index)
	{
		D_ASSERT(index < NUM_ATTACHMENTS, "Index out of range");
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_TextureAttachment[index]);
	}
	void OpenGLGBuffer::Blit()
	{
		glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	}
}
