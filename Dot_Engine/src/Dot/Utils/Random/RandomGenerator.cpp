#include "stdafx.h"
#include "RandomGenerator.h"


namespace Dot {

	double RandomGenerator::Random(double min, double max)
	{
		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_real_distribution<> dist(min,max);

		return dist(rng);
	}
}