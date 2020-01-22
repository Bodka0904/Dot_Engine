#pragma once
#include "Dot/Renderer/Camera/Camera.h"
#include "Dot/LevelEditor/Terrain/Terrain.h"
#include <glm/glm.hpp>

namespace Dot {

	class MousePicker
	{
	public:
		MousePicker();
		void CalculateMouseRay(const Camera& camera);
		const glm::vec3& TestIntersectionTerr(const Camera& camera,const Ref<Terrain>&terr);

		const glm::vec3& GetRay() const { return m_CurrentRay; }
	private:
		glm::vec3 binarySearch(const glm::vec3& camPos, const Ref<Terrain>& terr,float start, float finish, int count);
		glm::vec3 getPointOnRay(const glm::vec3& ray, float distance, const glm::vec3& camPos);
		
	private:
		glm::vec3 m_CurrentRay;
		float m_RayRange;
	};
}