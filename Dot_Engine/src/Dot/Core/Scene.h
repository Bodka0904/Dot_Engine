#pragma once
#include "Dot/ECS/Entity.h"

namespace Dot {
	class Scene
	{
	public:
		virtual void OnCreate() = 0;
		virtual void OnDestroy() = 0;
		virtual void OnActivate() =0;
		virtual void OnDeactivate()= 0;
		virtual void Update(float dt) = 0;
		virtual void Draw() = 0;

	private:
		std::vector<Entity> m_Entities;
	};
}