#pragma once

#include "Dot/ECS/ECSManager.h"
#include "Dot/PhysicsEngine/Components/ComponentData.h"

namespace Dot {

	class CollisionSystem : public System
	{
	public:
		CollisionSystem();

		// Simple swap and prune algorithm
		virtual void Update(float dt) override;

	private:
		void ProcessInteractions(float dt);
		bool Intersect(AABB& aabb1, AABB& aabb2);

		struct InteractionCompare
		{
			InteractionCompare(unsigned int axis)
				:m_Axis(axis)
			{}
			unsigned int m_Axis = 0;

			bool operator()(Entity a, Entity b)
			{
				auto& aabb1 = ECSManager::Get()->GetComponent<AABB>(a);
				auto& aabb2 = ECSManager::Get()->GetComponent<AABB>(b);
				return (aabb1.m_Min[m_Axis] < aabb2.m_Max[m_Axis]);
			}
		};
		InteractionCompare m_Cmp;
	};
}