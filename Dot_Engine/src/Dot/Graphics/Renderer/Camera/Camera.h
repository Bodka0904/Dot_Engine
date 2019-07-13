#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


namespace Dot {

	class Camera
	{
	public:
		Camera(float fov,float aspect,float zNear,float zFar);

		glm::vec3& GetPosition() { return m_Position; }
		void SetPosition(const glm::vec3 position) { m_Position = position; UpdateViewMatrix(); };

		glm::vec3& GetRotation() { return m_Rotation; }
		void SetRotation(const glm::vec3 rotation) { m_Rotation = rotation; UpdateViewMatrix(); };

		glm::vec3& GetTarget() { return m_Target; }
		void SetTarget(const glm::vec3 target) { m_Target = target; UpdateViewMatrix(); };

		void SetAspect(float aspect) { m_aspect = aspect; UpdateProjectionMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		void UpdateViewMatrix();
		
	private:
		
		void UpdateProjectionMatrix();
		 
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f,0.0f,0.0f };
		glm::vec3 m_Rotation = { 0.0f,0.0f,0.0f };
		glm::vec3 m_Target = { 0.0f,0.0f,1.0f };
		glm::vec3 m_Up = { 0.0f,1.0f,0.0f };
		
		float m_fov;
		float m_aspect;
		float m_zNear;
		float m_zFar;
	};
}