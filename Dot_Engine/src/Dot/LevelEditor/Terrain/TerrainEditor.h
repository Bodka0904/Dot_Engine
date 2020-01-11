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

	private:
		float m_Amplitude = 0.0f;
		float m_Roughness = 0.2f;
		float m_Height = 0.0f;
		int m_Seed = 0;

	private:
		std::future<void> m_Future;
		bool m_Processed = false;

	};
	
}