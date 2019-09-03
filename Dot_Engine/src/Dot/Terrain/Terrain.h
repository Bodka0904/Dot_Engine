#pragma once
#include "Dot/Graphics/Renderer/Buffers/ArrayBuffer.h"
#include <glm/glm.hpp>

namespace Dot {

	struct TerrainDataT
	{
		struct Vertex
		{
			glm::vec3 vertice;
			glm::vec3 normal;
			glm::vec2 texcoord;
		};

		std::vector<Vertex> m_vertices;
		std::shared_ptr<ArrayBuffer> m_vao;

		float m_size;
		int m_numvertex;
	};

	struct TerrainData
	{
		std::vector<std::vector<float> > heights;

		float size;
		int numvertex;
	};

	class Terrain
	{
	public:
		Terrain(TerrainData& data);
		~Terrain();

		float GetSize() const { return m_data.size; }
		float GetNumVertex() const { return m_data.numvertex; }

	private:
		TerrainData m_data;
	
	};
}

