#include "stdafx.h"
#include "CollisionSystem.h"

#include <glm/glm.hpp>
#include <algorithm>

namespace Dot {
	CollisionSystem::CollisionSystem()
		: m_Cmp(0)
	{
	}
	
	void CollisionSystem::processInteractions()
	{
		// Sort AABBs by min on highest variance of axis
		std::sort(m_Entities.begin(), m_Entities.end()-1, m_Cmp);
		
		// Find highest variance axis for AABBs
		glm::vec3 centerSum = glm::vec3(0);
		glm::vec3 centerSquareSum = glm::vec3(0);
	
		for (int i = 0; i < m_Entities.size(); ++i)
		{
			auto& aabb = ECSManager::Get()->GetComponent<AABB>(m_Entities[i]);
			centerSum += aabb.GetCenter();
			centerSquareSum += (aabb.GetCenter() * aabb.GetCenter());

			// Go through the list, test intersections in range		
			for (int j = i + 1; j < m_Entities.size(); ++j)
			{
				auto& aabbother = ECSManager::Get()->GetComponent<AABB>(m_Entities[j]);

				if (aabbother.m_Min[m_Cmp.m_Axis] > aabb.m_Max[m_Cmp.m_Axis])
				{
					break;
				}
				if (intersect(aabb, aabbother))
				{
					auto& rigidBody1 = ECSManager::Get()->GetComponent<RigidBody>(m_Entities[i]);
					rigidBody1.velocity = glm::vec3(0, 0, 0);

					auto& rigidBody2 = ECSManager::Get()->GetComponent<RigidBody>(m_Entities[j]);
					rigidBody2.velocity = glm::vec3(0, 0, 0);
				}
			}
		}
		centerSum /= m_Entities.size();
		centerSquareSum /= m_Entities.size();
		glm::vec3 variance = centerSquareSum - (centerSum * centerSum);

		float maxVariance = variance.x;
		int axis = 0;
		if (variance.y > maxVariance)
		{
			maxVariance = variance.x;
			axis = 1;
		}
		if (variance.z > maxVariance)
		{
			maxVariance = variance.z;
			axis = 2;
		}

		m_Cmp.m_Axis = axis;

	}
	void CollisionSystem::Update()
	{
		if (!m_Entities.empty())
			processInteractions();
	}

	void CollisionSystem::Add(Entity entity)
	{
		LOG_INFO("Entity with ID % added", entity);
		m_Entities.push_back(entity);
	}

	void CollisionSystem::Remove(Entity entity)
	{
		std::sort(m_Entities.begin(), m_Entities.end());

		int position = binarySearch(0, (int)m_Entities.size() - 1, entity);
		if (position != -1 && !m_Entities.empty())
		{
			LOG_INFO("Entity with ID % removed", entity);
			m_Entities[position] = m_Entities[m_Entities.size() - 1];
			m_Entities.pop_back();
		}
	}
	
	bool CollisionSystem::intersect(AABB& aabb1, AABB& aabb2)
	{
		return  (aabb1.m_Min[0] <= aabb2.m_Max[0] && aabb1.m_Max[0] >= aabb2.m_Min[0]) &&
				(aabb1.m_Min[1] <= aabb2.m_Max[1] && aabb1.m_Max[1] >= aabb2.m_Min[1]) &&
				(aabb1.m_Min[2] <= aabb2.m_Max[2] && aabb1.m_Max[2] >= aabb2.m_Min[2]);
	}
	int CollisionSystem::binarySearch(int start, int end, Entity entity)
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