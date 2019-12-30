#include "stdafx.h"
#include "PhysicsSystem.h"

namespace Dot {
	void PhysicsSystem::Update(float dt)
	{
		for (int i = 0; i < m_Entities.size(); ++i)
		{
			auto& transform = ECSManager::Get()->GetComponent<Transform>(m_Entities[i]);
			auto& rigidBody = ECSManager::Get()->GetComponent<RigidBody>(m_Entities[i]);

			transform.m_Pos += rigidBody.m_Velocity * dt;
		}
	}
}
