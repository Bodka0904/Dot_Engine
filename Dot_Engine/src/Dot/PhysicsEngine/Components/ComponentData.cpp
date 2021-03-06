#include "stdafx.h"
#include "ComponentData.h"

namespace Dot {

	void AABB::Tranlate(const glm::vec3& translation)
	{
		m_Min[0] += translation.x;
		m_Min[1] += translation.y;
		m_Min[2] += translation.z;

		m_Max[0] += translation.x;
		m_Max[1] += translation.y;
		m_Max[2] += translation.z;
	}
	void AABB::Expand(const glm::vec3& size)
	{
	}
	void AABB::MoveToDest(const glm::vec3& dest)
	{
		glm::vec3 center = GetCenter();

		m_Min[0] += dest.x - center.x;
		m_Min[1] += dest.y - center.y;
		m_Min[2] += dest.z - center.z;
	}
	const glm::vec3& Dot::AABB::GetCenter() const
	{
		return (glm::vec3(m_Max[0], m_Max[1], m_Max[2]) + glm::vec3(m_Min[0], m_Min[1], m_Min[2])) * 0.5f;
	}
}
