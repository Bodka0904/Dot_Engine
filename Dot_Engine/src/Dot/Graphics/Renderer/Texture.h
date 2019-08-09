#pragma once

namespace Dot {
	
	class Texture
	{
	public:
		Texture();
		~Texture();
		void Create2D(const std::string& fileName);
		void CreateCubeMap(const std::vector<std::string>faces);

		void Bind(unsigned int unit);

	private:
		unsigned int texture;
		int textureType;
	};

}