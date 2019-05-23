#pragma once
#include "Transform.h"
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>




class Camera
{
public:
	Camera(glm::mat4& projectionMatrix = glm::perspective(70.0f,1280.0f/720.0f,0.01f,1000.0f))
		:
		m_projectionMatrix(projectionMatrix),
		m_position(glm::vec3(0, 0, 0)),
		m_rotation(glm::vec3(0, 0, 0)),
		m_up(glm::vec3(0, 1, 0)),
		m_cameraTarget(glm::vec3(0, 0, -1))
	{
	}

	inline glm::mat4 GetViewProjection() const 
	{
		glm::mat4 posMatrix = glm::translate(m_position);
		glm::mat4 rotXMatrix = glm::rotate(m_rotation.x, glm::vec3(1, 0, 0));
		glm::mat4 rotYMatrix = glm::rotate(m_rotation.y, glm::vec3(0, 1, 0));
		glm::mat4 rotZMatrix = glm::rotate(m_rotation.z, glm::vec3(0, 0, 1));

		glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;


		return m_projectionMatrix * glm::lookAt(m_position,m_position + m_cameraTarget, m_up)  * posMatrix * rotMatrix;
	}
	
	inline const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
	inline void SetProjectionMatrix(glm::mat4 projectionMatrix) { m_projectionMatrix = projectionMatrix; }
	inline glm::vec3& GetPos() { return m_position; }
	inline glm::vec3& GetRot() { return m_rotation; }
	inline glm::vec3& GetCameraTarget() { return m_cameraTarget; }
	inline glm::vec3& GetUp() { return m_up; }

	




protected:
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_up;
	glm::vec3 m_cameraTarget;



};