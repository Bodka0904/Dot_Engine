#pragma once


namespace Dot {
	class GBuffer
	{
	public:
		virtual ~GBuffer() = default;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void BindTexture(unsigned int unit, unsigned int index) = 0;
		virtual void Blit() = 0;
		static Ref<GBuffer> Create(uint32_t width, uint32_t height);
	};
}