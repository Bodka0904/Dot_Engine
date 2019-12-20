#pragma once
#include "Dot/Renderer/Camera/Camera.h"
#include "Dot/LevelEditor/Terrain/Terrain.h"
#include <glm/glm.hpp>

namespace Dot {

	class MousePicker
	{
	public:
		MousePicker();
		void CalculateMouseRay(Camera& camera);
		bool TestIntersection(Terrain& terrain,Camera& camera,glm::vec3& pos);
	private:
		glm::vec3 binarySearch(const glm::vec3& camPos,float terrHeight,float start, float finish, int count);	
		glm::vec3 getPointOnRay(const glm::vec3& ray, float distance, const glm::vec3& camPos);
	
	private:
		glm::vec3 m_CurrentRay;
	};
}