#include "stdafx.h"
#include "CameraController.h"

#include "Dot/Core/Input.h"
#include "Dot/Core/KeyCodes.h"
#include "Dot/Core/MouseButtonCodes.h"

namespace Dot {
	CameraController::CameraController(const glm::mat4& projectionMatrix)
		: m_Camera(projectionMatrix),m_InitialMousePosition(0)
	{
		m_Camera.Update();
	}
	void CameraController::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(D_KEY_LEFT_ALT))
		{
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = mouse - m_InitialMousePosition;
			m_InitialMousePosition = mouse;
			delta *= ts;

			if (Input::IsMouseButtonPressed(D_MOUSE_BUTTON_MIDDLE))
				m_Camera.MousePan(delta);
			else if (Input::IsMouseButtonPressed(D_MOUSE_BUTTON_LEFT))
				m_Camera.MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(D_MOUSE_BUTTON_RIGHT))
				m_Camera.MouseZoom(delta.y);		
			
			m_Camera.Update();
		}
	
	}
	void CameraController::OnEvent(Event& e)
	{
		
	}
}