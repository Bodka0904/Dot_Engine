#pragma once
#include "Dot/Renderer/Camera/Camera.h"
#include <glm/glm.hpp>

namespace Dot {

	class MousePicker
	{
	public:
		MousePicker();

		void CalculateMouseRay(Camera& camera);
		const glm::vec3& GetRay() const { return m_CurrentRay; }

	private:
		glm::vec3 m_CurrentRay;

	};
}