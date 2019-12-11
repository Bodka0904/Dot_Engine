#pragma once


namespace Dot {
	enum class FramebufferFormat
	{
		None = 0,
		RGB = 1,
		RGBA8 = 2,
		RGBA16F = 3
	};

	class Framebuffer
	{
	public:
		Framebuffer(uint32_t width, uint32_t height, FramebufferFormat format);
		~Framebuffer();

		void Resize(uint32_t width, uint32_t height);

		void Bind() const ;
		void Unbind() const;

		void BindTexture(uint32_t slot = 0) const;

		unsigned int GetFBO() const { return m_FBO; }
		unsigned int GetColorAttachment() const { return m_ColorAttachment; }
		unsigned int GetDepthAttachment() const { return m_DepthAttachment; }

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		FramebufferFormat GetFormat() const { return m_Format; }
	private:
		unsigned int m_FBO;

		uint32_t m_Width, m_Height;
		FramebufferFormat m_Format;

		unsigned int m_ColorAttachment, m_DepthAttachment;
	};



}