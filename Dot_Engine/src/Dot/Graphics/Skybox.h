#pragma once
#include "Renderer/Texture.h"
#include "Renderer/Buffers/ArrayBuffer.h"

namespace Dot {
	class Skybox
	{
	public:
		Skybox(const std::vector<std::string> faces,int SIZE);
		~Skybox();

		Ref<CubeMapTexture>& GetTexture() { return m_CubeMap; }
		Ref<ArrayBuffer> GetVao() { return m_VAO; }

	private:	
		Ref<CubeMapTexture> m_CubeMap;
		Ref<ArrayBuffer>m_VAO;

	};

}