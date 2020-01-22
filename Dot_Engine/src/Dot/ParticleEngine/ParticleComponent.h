#pragma once
#include "Dot/Renderer/Shader/Shader.h"
#include <glm/glm.hpp>

namespace Dot {
	struct ParticleEmitter
	{
		int count;
		int maxCount;
		float speed;
		float gravity;
		float minLifeSpan;
		float maxLifeSpan;
	};
	struct SizeOverLifeTime
	{
		enum
		{
			BIGGER,
			SMALLER,
			NONE
		};
		int sizeOverLifeTime;
	};
	struct VelocityOverLifeTime
	{
		float speedMultiplier;
	};
	struct ComputeComponent
	{
		Ref<Shader> shader;
	};
}