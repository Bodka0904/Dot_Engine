#include "stdafx.h"
#include "Terrain.h"
#include "Dot/Graphics/Renderer/Buffer.h"
#include <glm/glm.hpp>
#include "Dot/Utils/BmpLoader.h"
#include <random>

#define _USE_MATH_DEFINES
#include <math.h>


namespace Dot {

	Terrain::Terrain(TerrainData& data)
		:m_data(data)
	{
	}


	Terrain::~Terrain()
	{
	}


}