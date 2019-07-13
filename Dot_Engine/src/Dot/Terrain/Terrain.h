#pragma once
#include "Dot/Graphics/Renderer/ArrayBuffer.h"
#include <glm/glm.hpp>

namespace Dot {
	struct TerrainData
	{
		std::shared_ptr<ArrayBuffer> VAO;
		std::vector<std::vector<float> > heights;

		float size;
		int numvertex;
	};

	class Terrain
	{
	public:
		Terrain(TerrainData& data);
		~Terrain();


		std::shared_ptr<ArrayBuffer>GetVao() const { return m_data.VAO; }
		float GetSize() const { return m_data.size; }
		float GetNumVertex() const { return m_data.numvertex; }

	private:
		TerrainData m_data;
	
	};
}

