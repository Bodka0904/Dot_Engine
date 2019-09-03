#pragma once
#include "Dot/Graphics/Renderer/Buffers/ArrayBuffer.h"
#include "Terrain.h"
#include <glm/glm.hpp>

namespace Dot {

	class TerrainEditor
	{
	public:
		struct Vertex
		{
			glm::vec3 vertice;
			glm::vec3 normal;
			glm::vec2 texcoord;
		};

		TerrainEditor();
		~TerrainEditor();

		void GenFlatTerrain(float size, unsigned int numvertex);

		void ApplyHeightsValueNoise(int height);
		void ApplyNormals();
		void SetSeed(int num) { seed = num; }
		void Draw();
		

		float &GetAmplitude() { return AMPLITUDE; }
		int &GetOctaves() { return OCTAVES; }
		float &GetRoughness() { return ROUGHNESS; }

		std::shared_ptr<Terrain> CreateTerrain() const;

	private:
		float getNoise(int x, int z);
		float getSmoothNoise(int x, int z);
		float getInterpolatedNoise(float x, float z);
		float interpolateCosine(float a, float b, float blend);
		float generateHeight(int x, int z);
		float getHeight(int x, int z) const;

		glm::vec3 generateNormal(int x, int z) const;

	private:
	
		std::shared_ptr<TerrainData> m_data;
		std::shared_ptr<ArrayBuffer> m_VAO;
		std::vector<Vertex> m_vertices;


	private:
		float AMPLITUDE = 5.0f;
		int OCTAVES = 4;
		float ROUGHNESS = 0.2f;

		int seed;
	};

}

