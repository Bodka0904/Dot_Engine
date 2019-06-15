#pragma once
#include <glm/glm.hpp>

class ParticleEffect
{
public:
	ParticleEffect();
	virtual ~ParticleEffect();
	virtual void Init() = 0;

	virtual void Update() = 0;
	virtual void Draw() = 0;
};

