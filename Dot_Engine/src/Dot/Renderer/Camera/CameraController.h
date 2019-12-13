#pragma once
#include "Camera.h"
#include "Dot/Utils/Time/Timestep.h"
#include "Dot/Core/Events/Event.h"

#include <glm/glm.hpp>

namespace Dot {

	class CameraController
	{
	public:
		CameraController(const glm::mat4& projectionMatrix);
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		Camera& GetCamera() { return m_Camera; };
	private:
		Camera m_Camera;
		glm::vec2 m_InitialMousePosition;
	};
}