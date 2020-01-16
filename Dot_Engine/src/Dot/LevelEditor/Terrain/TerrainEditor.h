#pragma once
#include "Terrain.h"
//TEMPORARY//
#include <future>

namespace Dot {

	class TerrainEditor
	{
	public:
		TerrainEditor();	
		void UpdateTerrain(Ref<Terrain> terrain);

		// Function to update Terrain 
		void DiamondSquare();
		void HeightsValueNoise();

		void ApplyNormals();
		void ApplyHeights();
	private:
		// Helper functions 
		glm::vec3 generateNormal(int x, int z) const;
		float getNoise(int x, int z);
		float getSmoothNoise(int x, int z);
		float getInterpolatedNoise(float x, float z);
		float interpolateCosine(float a, float b, float blend);
		float generateHeight(int x, int z);
		float getHeight(int x, int z) const;

	

		void diamondSquare(int size);
		void squareStep(int x, int z, int reach);
		void diamondStep(int x, int z, int reach);
	private:

	private:
		std::future<void> m_Future;
		bool m_Processed = false;

	};
	
}