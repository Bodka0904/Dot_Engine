#pragma once
#include "Dot/Renderer/Texture.h"
#include "Dot/Renderer/Buffers/ArrayBuffer.h"

namespace Dot {
	class Skybox
	{
	public:
		Skybox(const std::vector<std::string> faces,float SIZE);
		~Skybox();

		Ref<CubeMapTexture>& GetTexture() { return m_CubeMap; }
		Ref<ArrayBuffer> GetVAO() { return m_VAO; }

	private:	
		Ref<CubeMapTexture> m_CubeMap;
		Ref<ArrayBuffer>m_VAO;

	};

}