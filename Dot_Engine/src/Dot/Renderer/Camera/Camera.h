#pragma once
#include <glm/glm.hpp>

namespace Dot {

	class Camera
	{
	public:
		Camera(const glm::mat4& projectionMatrix);
		void Update();
		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		inline void SetDistance(float distance) { m_Distance = distance; }
		inline void SetProjectionMatrix(const glm::mat4& projectionMatrix) { m_ProjectionMatrix = projectionMatrix; }
		inline void SetPosition(const glm::vec3& pos) { m_Position = pos; }
		inline void InvertPitch() { m_Pitch = -m_Pitch; }

		inline float GetDistance() const { return m_Distance; }
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; }
		
		glm::vec3 GetUpDirection();
		glm::vec3 GetRightDirection();
		glm::vec3 GetForwardDirection();
		glm::vec3& GetRotation() { return m_Rotation; };
		const glm::vec3& GetPosition() const { return m_Position; }
		
	
	private:
		glm::vec3 CalculatePosition();
		glm::quat GetOrientation();
	private:
		glm::mat4 m_ProjectionMatrix, m_ViewMatrix;
		glm::vec3 m_Position, m_Rotation, m_FocalPoint;
		glm::vec3 m_InitialFocalPoint, m_InitialRotation;

		float m_Distance;
		float m_PanSpeed, m_RotationSpeed, m_ZoomSpeed;

		float m_Pitch, m_Yaw;
	};

}