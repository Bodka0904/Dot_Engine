#include "stdafx.h"
#include "SAP.h"

#include <algorithm>

namespace Dot {
	void InteractionEngine::Push(Ref<Entity> entity)
	{
		m_Entities.push_back(entity);
	}
	void InteractionEngine::ProcessInteractions(float dt)
	{
		// Sort AABBs by min on highest variance of axis
		std::sort(m_Entities.begin(), m_Entities.end(), m_Cmp);

		// Find highest variance axis for AABBs
		glm::vec3 centerSum = glm::vec3(0);
		glm::vec3 centerSquareSum = glm::vec3(0);
		for (int i = 0; i < m_Entities.size(); ++i)
		{
			centerSum += m_Entities[i]->m_AABB.GetCenter();
			centerSquareSum += (m_Entities[i]->m_AABB.GetCenter() * m_Entities[i]->m_AABB.GetCenter());

			// Go through the list, test intersections in range
			for (int j = i - 1; j < m_Entities.size(); ++j)
			{
				// If true nothing else can intersect
				if (m_Entities[j]->m_AABB.GetMin()[m_Cmp.m_Axis] > m_Entities[i]->m_AABB.GetMax()[m_Cmp.m_Axis])
				{
					break;
				}
				if (Intersect(i, j))
				{
					std::cout << "Intersection" << std::endl;
				}
			}
		}
		centerSum /= m_Entities.size();
		centerSquareSum /= m_Entities.size();
		glm::vec3 variance = centerSquareSum - (centerSum*centerSum);
		
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
	void InteractionEngine::Remove(int index)
	{
		std::swap(m_Entities[index], m_Entities[m_Entities.size() - 1]);
		m_Entities.pop_back();
	}
	bool InteractionEngine::Intersect(unsigned int firstIndex, unsigned int secondIndex)
	{
		if (m_Entities[firstIndex]->m_AABB.GetMax()[0] < m_Entities[secondIndex]->m_AABB.GetMin()[0])
			return false;
		if (m_Entities[secondIndex]->m_AABB.GetMax()[0] < m_Entities[firstIndex]->m_AABB.GetMin()[0])
			return false;

		if (m_Entities[firstIndex]->m_AABB.GetMax()[1] < m_Entities[secondIndex]->m_AABB.GetMin()[1])
			return false;
		if (m_Entities[secondIndex]->m_AABB.GetMax()[1] < m_Entities[firstIndex]->m_AABB.GetMin()[1])
			return false;
		
		if (m_Entities[firstIndex]->m_AABB.GetMax()[2] < m_Entities[secondIndex]->m_AABB.GetMin()[2])
			return false;
		if (m_Entities[secondIndex]->m_AABB.GetMax()[2] < m_Entities[firstIndex]->m_AABB.GetMin()[2])
			return false;

		return true;
	}
}