#pragma once
#include "Renderer/Texture.h"
#include "Renderer/ArrayBuffer.h"

namespace Dot {
	class Skybox
	{
	public:
		Skybox(const std::vector<std::string> faces,int SIZE);
		~Skybox();

		Texture& GetTexture() { return m_CubeMap; }
		std::shared_ptr<ArrayBuffer> GetVao() { return m_VAO; }

	private:	
		Texture m_CubeMap;
		std::shared_ptr<ArrayBuffer>m_VAO;

	};

}