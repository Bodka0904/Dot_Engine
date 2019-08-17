#include "stdafx.h"
#include "MousePicker.h"
#include "Dot/Input.h"
#include "Dot/Application.h"
#include <glm/gtx/transform.hpp>

namespace Dot {
	MousePicker::MousePicker()
	{
	}

	void MousePicker::CalculateMouseRay(Camera& camera)
	{

		Application& app = Application::Get();
		

		float mouseX = Input::GetMouseX();
		float mouseY = Input::GetMouseY();;
		float mouseZ = 1.0f;

		mouseX = (2.0f * mouseX) / app.GetWin().GetWidth() - 1.0f;
		mouseY = 1.0f - (2.0f * mouseY) / app.GetWin().GetHeight();
		

		glm::vec4 clipCoords = glm::vec4(mouseX, mouseY, -1.0f, 1.0f);

		glm::vec4 rayEye = glm::inverse(camera.GetProjectionMatrix()) * clipCoords;
		rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

		glm::vec3 rayWorld = glm::inverse(camera.GetViewMatrix()) * rayEye;
		rayWorld = glm::normalize(rayWorld);

		std::cout << rayWorld.x << " " << rayWorld.y << " " << rayWorld.z << std::endl;
	}

}
