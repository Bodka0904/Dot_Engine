#pragma once



namespace Dot {
	struct TextureData
	{
		unsigned int texture;
		unsigned char* imageData;
		int width;
		int height;
		int numComponents;
	};

	class Texture
	{
	public:
		Texture();
		virtual ~Texture();

		void Create(std::string fileName);
		void Bind(unsigned int unit);

	private:
		TextureData data;

	};

}