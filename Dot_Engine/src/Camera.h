#pragma once
#include <iostream>
#include "Transform.h"
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "GL\glew.h"


class Camera
{
public:
	Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar)
	{
		m_perspective = glm::perspective(fov, aspect, zNear, zFar);
		m_position = pos;
		m_forward = glm::vec3(0, 0, 1);
		m_up = glm::vec3(0, 0.5, 0);

	}

	inline glm::mat4 GetViewProjection() const
	{
		return m_perspective * glm::lookAt(m_position, cameraCenter, m_up);
	}

	//WHEN WINDOW IS RESIZED UPDATE PERSPECTIVE WITH NEW WIDTH AND HEIGHT - SHOULD CHECK IN EVENT HANDLER
	glm::mat4& GetPerspective() { return m_perspective; }
	glm::vec3& GetPos() { return m_position; }
	glm::vec3& GetForward() { return m_forward; }
	glm::vec3& GetUp() { return m_up; }
	


private:

	glm::mat4 m_perspective;
	glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec3 m_up;
	glm::vec3 cameraCenter;
	








};