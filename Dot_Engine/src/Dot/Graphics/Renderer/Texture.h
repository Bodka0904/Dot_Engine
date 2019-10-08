#pragma once

namespace Dot {


	class Texture
	{
	public:
		Texture(const std::string& fileName);
		Texture(const std::vector<std::string>faces);
		~Texture();

		void Bind(unsigned int unit);
	
	private:
		int CalculateMipMapCount(int width, int height);
	
	private:
		unsigned int texture;
		int textureType;
	};


	class CubeMapTexture
	{
	public:
		CubeMapTexture(const std::vector<std::string>faces);
		~CubeMapTexture();

		void Bind(unsigned int unit);

	private:
		unsigned int texture;
		int textureType;

	};
}