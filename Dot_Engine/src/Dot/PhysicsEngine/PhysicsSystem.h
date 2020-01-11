#pragma once
#include "Dot/PhysicsEngine/Components/ComponentData.h"
#include "Dot/ECS/ECSManager.h"

namespace Dot {
	class PhysicsSystem : public System
	{
	public:
		virtual void Update(float dt);
		virtual void Add(Entity entity) override;
		virtual void Remove(Entity entity) override;
	private:
		int binarySearch(int start, int end, Entity entity);
	private:
		std::vector<Entity> m_Entities;
	};
}