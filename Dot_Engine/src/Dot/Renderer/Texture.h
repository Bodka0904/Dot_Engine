#pragma once


namespace Dot {

	class Texture2D
	{
	public:
		virtual ~Texture2D() = default;
		
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;
		virtual void Bind(unsigned int unit) = 0;
		
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path,bool filters = false,bool flipped = true);
	};


	class CubeMapTexture
	{
	public:
		virtual ~CubeMapTexture() = default;

		virtual void Bind(unsigned int unit) = 0;

		static Ref<CubeMapTexture> Create(const std::vector<std::string>&faces);
	};
}