#include "stdafx.h"
#include "MousePicker.h"
#include "Dot/Core/Input.h"
#include "Dot/Core/Application.h"

#include <glm/gtx/transform.hpp>

#define RAY_RECURSION_COUNT 200
#define RAY_RANGE 600

namespace Dot {
	MousePicker::MousePicker()
	{
	}

	void MousePicker::CalculateMouseRay(const Camera& camera)
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
		m_CurrentRay = rayWorld;
	}

	bool MousePicker::TestIntersection(const Camera& camera, glm::vec3& pos, float height)
	{		
		glm::vec3 startPoint = getPointOnRay(m_CurrentRay, 0, camera.GetPosition());
		glm::vec3 endPoint = getPointOnRay(m_CurrentRay, RAY_RANGE, camera.GetPosition());
		if (startPoint.y >= height && endPoint.y < height)
		{
			pos = binarySearch(camera.GetPosition(), height, 0, RAY_RANGE, 0);
			return true;
		}
		return false;
	}

	glm::vec3 MousePicker::binarySearch(const glm::vec3& camPos, float terrHeight, float start, float finish, int count)
	{
		float half = start + ((finish - start) / 2.0f);
		if (count >= RAY_RECURSION_COUNT)
		{
			glm::vec3 scaledRay = glm::vec3(m_CurrentRay.x * half, m_CurrentRay.y * half, m_CurrentRay.z * half);
			return glm::vec3(camPos.x + scaledRay.x, camPos.y + scaledRay.y, camPos.z + scaledRay.z);
		}	
		glm::vec3 startPoint = getPointOnRay(m_CurrentRay, start, camPos);
		glm::vec3 endPoint = getPointOnRay(m_CurrentRay, half, camPos);

		if (startPoint.y >= terrHeight && endPoint.y < terrHeight)
		{
			return binarySearch(camPos, terrHeight,start, half, count + 1);
		}
		else
		{
			return binarySearch(camPos, terrHeight, half, finish, count + 1);
		}
	}

	glm::vec3 MousePicker::getPointOnRay(const glm::vec3& ray, float distance, const glm::vec3& camPos)
	{
		glm::vec3 scaledRay = glm::vec3(ray.x * distance, ray.y * distance, ray.z * distance);
		glm::vec3 result = glm::vec3(camPos.x + scaledRay.x, camPos.y + scaledRay.y, camPos.z + scaledRay.z);
		return result;
	}

	

}
