#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace Dot {
	class AABB
	{
	public:
		void Tranlate(const glm::vec3& translation);
		void Expand(const glm::vec3& size);
		void MoveToDest(const glm::vec3& dest);
		const glm::vec3& GetCenter() const;

		float m_Min[3];
		float m_Max[3];
	};

	struct Transform
	{
		glm::mat4 GetModel()
		{
			glm::mat4 posMatrix = glm::translate(m_Pos);
			glm::mat4 rotXMatrix = glm::rotate(m_Rot.x, glm::vec3(1, 0, 0));
			glm::mat4 rotYMatrix = glm::rotate(m_Rot.y, glm::vec3(0, 1, 0));
			glm::mat4 rotZMatrix = glm::rotate(m_Rot.z, glm::vec3(0, 0, 1));
			glm::mat4 scaleMatrix = glm::scale(m_Scale);

			glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;
			return posMatrix * rotMatrix * scaleMatrix;
		}

		glm::vec3 m_Pos;
		glm::vec3 m_Rot;
		glm::vec3 m_Scale;
	};

	struct Transform2D
	{
		glm::vec2 m_Pos;
		glm::vec2 m_Rot;
		glm::vec2 m_Scale;
	};

	struct RigidBody
	{
		glm::vec3 m_Velocity;
		//Probably add weight and so on
	};

}