#pragma once
#include "Dot/Renderer/Camera/Camera.h"
#include <glm/glm.hpp>

namespace Dot {

	class MousePicker
	{
	public:
		MousePicker();
		void CalculateMouseRay(const Camera& camera);
		bool TestIntersection(const Camera& camera,glm::vec3& pos,float height);

		const glm::vec3& GetRay() const { return m_CurrentRay; }
	private:
		glm::vec3 binarySearch(const glm::vec3& camPos,float terrHeight,float start, float finish, int count);	
		glm::vec3 getPointOnRay(const glm::vec3& ray, float distance, const glm::vec3& camPos);
	
	private:
		glm::vec3 m_CurrentRay;
	};
}