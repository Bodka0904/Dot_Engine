#include "stdafx.h"
#include "PhysicsSystem.h"

namespace Dot {
	void PhysicsSystem::Update(float dt)
	{
		for (int i = 0; i < m_Entities.size(); ++i)
		{
			auto transform = &ECSManager::Get()->GetComponent<Transform>(m_Entities[i]);
			auto& rigidBody = ECSManager::Get()->GetComponent<RigidBody>(m_Entities[i]);

			transform->pos += rigidBody.velocity * dt;

			transform->UpdateModel();
		}
	}
	void PhysicsSystem::Add(Entity entity)
	{
		LOG_INFO("Entity with ID %d added",entity);
		m_Entities.push_back(entity);
	}
	void PhysicsSystem::Remove(Entity entity)
	{
		std::sort(m_Entities.begin(), m_Entities.end());

		int position = binarySearch(0, (int)m_Entities.size() - 1, entity);
		if (position != -1 && !m_Entities.empty())
		{
			LOG_INFO("Entity with ID %d removed",entity);
			m_Entities[position] = m_Entities[m_Entities.size() - 1];
			m_Entities.erase(m_Entities.end() - 1);
		}
	}
	int PhysicsSystem::binarySearch(int start, int end, Entity entity)
	{
		if (end >= start)
		{
			int mid = start + (end - start) / 2;
			if (m_Entities[mid] == entity)
				return mid;

			if (m_Entities[mid] > entity)
				return binarySearch(start, mid - 1, entity);


			return binarySearch(mid + 1, end, entity);
		}
		return -1;
	}
}
