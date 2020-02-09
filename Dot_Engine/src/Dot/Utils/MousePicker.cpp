#include "stdafx.h"
#include "MousePicker.h"
#include "Dot/Core/Input.h"
#include "Dot/Core/Application.h"

#include <glm/gtx/transform.hpp>

#define RAY_RECURSION_COUNT 200
#define RAY_RANGE 400

namespace Dot {
	MousePicker::MousePicker()
		: m_CurrentRay(glm::vec3(0))
	{
		m_RayRange = 400;
	}

	void MousePicker::CalculateMouseRay(const Camera& camera)
	{
		Application& app = Application::Get();
	
		float mouseX = Input::GetMouseX();
		float mouseY = Input::GetMouseY();;
		float mouseZ = 1.0f;

		mouseX = mouseX / (app.GetWin().GetWidth() * 0.5f) - 1.0f;
		mouseY = mouseY / (app.GetWin().GetHeight() * 0.5f)- 1.0f;
		
		glm::mat4 invVP = glm::inverse(camera.GetProjectionMatrix() * camera.GetViewMatrix());
		glm::vec4 screenPos = glm::vec4(mouseX, -mouseY, 1.0f, 1.0f);
		glm::vec4 worldPos = invVP * screenPos;
		glm::vec3 dir = glm::normalize(glm::vec3(worldPos));
		m_CurrentRay = dir;

		//glm::vec4 clipCoords = glm::vec4(mouseX, -mouseY, -1.0f, 1.0f);
		//glm::vec4 rayEye = glm::inverse(camera.GetProjectionMatrix()) * clipCoords;
		//rayEye = glm::vec4(rayEye.x, rayEye.y,-1.0f, 0.0f);
		//
		//glm::vec3 rayWorld = glm::inverse(camera.GetViewMatrix()) * rayEye;
		//rayWorld = glm::normalize(rayWorld);
		//m_CurrentRay = rayWorld;
	}

	const glm::vec3& MousePicker::TestIntersectionTerr(const Camera& camera, const Ref<Terrain>& terr)
	{		
		m_RayRange = RAY_RANGE + camera.GetPosition().y;
		return binarySearch(camera.GetPosition(), terr, 0, m_RayRange, 0);
	}

	glm::vec3 MousePicker::binarySearch(const glm::vec3& camPos, const Ref<Terrain>& terr, float start, float finish, int count)
	{
		float half = start + ((finish - start) / 2.0f);
		glm::vec3 startPoint = getPointOnRay(m_CurrentRay, start, camPos);
		glm::vec3 endPoint = getPointOnRay(m_CurrentRay, half, camPos);
		//float height = terr->GetHeight(endPoint);
		float height = 0.0f;
		if (count >= RAY_RECURSION_COUNT)
		{
			glm::vec3 result = getPointOnRay(m_CurrentRay, half, camPos);
			result.y = height;
			return getPointOnRay(m_CurrentRay, half, camPos);
		}		
		else if (startPoint.y >= height && endPoint.y < height)
		{
			return binarySearch(camPos, terr,start, half, count + 1);
		}
		else
		{
			return binarySearch(camPos, terr, half, finish, count + 1);
		}
	}

	glm::vec3 MousePicker::getPointOnRay(const glm::vec3& ray, float distance, const glm::vec3& camPos)
	{
		glm::vec3 scaledRay = glm::vec3(ray.x * distance, ray.y * distance, ray.z * distance);
		glm::vec3 result = glm::vec3(camPos.x + scaledRay.x, camPos.y + scaledRay.y, camPos.z + scaledRay.z);
		return result;
	}
}
