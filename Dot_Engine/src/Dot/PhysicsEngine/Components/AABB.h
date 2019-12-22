#pragma once

#include <glm/glm.hpp>

namespace Dot {
	class AABB
	{
	public:
		AABB(const glm::vec3& min, const glm::vec3& max);

		void Tranlate(const glm::vec3& translation);
		void Expand(const glm::vec3& size);
		void MoveToDest(const glm::vec3& dest);

		const float* GetMin() const { return m_Min; }
		const float* GetMax() const { return m_Max; }
		const glm::vec3& GetCenter() const;
	private:
		float m_Min[3];
		float m_Max[3];
	};
}