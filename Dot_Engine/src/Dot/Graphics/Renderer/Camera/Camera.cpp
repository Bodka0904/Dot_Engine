#include "stdafx.h"
#include "Camera.h"
#include <glm/gtx/transform.hpp>

namespace Dot {
	Camera::Camera(float fov, float aspect, float zNear, float zFar)
		: 
		m_ProjectionMatrix(glm::perspective(glm::radians(fov),aspect,zNear,zFar)),
		m_ViewMatrix(1.0f),
		m_fov(fov),
		m_aspect(aspect),
		m_zNear(zNear),
		m_zFar(zFar)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	void Camera::UpdateViewMatrix()
	{
		
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Target, m_Up);
		
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	void Camera::UpdateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_zNear, m_zFar);
	}
}