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
		~Framebuffer() = default;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void BindTexture(uint32_t slot = 0) const = 0;

		virtual unsigned int GetFBO() const = 0;
		virtual unsigned int GetColorAttachment() const = 0;
		virtual unsigned int GetDepthAttachment() const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual FramebufferFormat GetFormat() const = 0;
		
		Ref<Framebuffer> Create(uint32_t width, uint32_t height, FramebufferFormat format);
	};



}