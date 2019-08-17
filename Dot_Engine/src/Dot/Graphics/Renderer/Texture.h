#pragma once

namespace Dot {
	
	enum class TextureFormat
	{
		None = 0,
		RGB,
		RGBA
	};

	class Texture
	{
	public:
		Texture();
		~Texture();
		void Create2D(const std::string& fileName,TextureFormat format);
		void CreateCubeMap(const std::vector<std::string>faces);

		void Bind(unsigned int unit);
	
	private:
		int CalculateMipMapCount(int width, int height);
	
	private:
		unsigned int texture;
		int textureType;
	};

}