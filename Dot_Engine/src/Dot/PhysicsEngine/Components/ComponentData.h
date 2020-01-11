#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "Dot/Renderer/Camera/Camera.h"

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
		void UpdateModel()
		{
			glm::mat4 posMatrix = glm::translate(pos);
			glm::mat4 rotXMatrix = glm::rotate(rot.x, glm::vec3(1, 0, 0));
			glm::mat4 rotYMatrix = glm::rotate(rot.y, glm::vec3(0, 1, 0));
			glm::mat4 rotZMatrix = glm::rotate(rot.z, glm::vec3(0, 0, 1));
			glm::mat4 scaleMatrix = glm::scale(scale);

			glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;
			model = posMatrix * rotMatrix * scaleMatrix;
		}
		void UpdateRotation()
		{
			glm::mat4 rotXMatrix = glm::rotate(rot.x, glm::vec3(1, 0, 0));
			glm::mat4 rotYMatrix = glm::rotate(rot.y, glm::vec3(0, 1, 0));
			glm::mat4 rotZMatrix = glm::rotate(rot.z, glm::vec3(0, 0, 1));
			model *= rotZMatrix * rotYMatrix * rotXMatrix;
		}
		void UpdateTranslation()
		{
			model *= glm::translate(pos);
		}
		void UpdateScale()
		{
			model *= glm::scale(scale);
		}
		void FaceCamera(const Camera& camera)
		{
			UpdateModel();
			glm::mat4 viewMat = camera.GetViewMatrix();

			model[0][0] = viewMat[0][0];
			model[0][1] = viewMat[1][0];
			model[0][2] = viewMat[2][0];
			
			model[1][0] = viewMat[0][1];
			model[1][1] = viewMat[1][1];
			model[1][2] = viewMat[2][1];
			
			model[2][0] = viewMat[0][2];
			model[2][1] = viewMat[1][2];
			model[2][2] = viewMat[2][2];
	
			UpdateRotation();
			UpdateScale();		
		}
		glm::vec3 pos = glm::vec3(0);
		glm::vec3 rot = glm::vec3(0);
		glm::vec3 scale = glm::vec3(1);
		glm::mat4 model = glm::mat4(1);
	};

	struct Transform2D
	{
		glm::vec2 pos;
		glm::vec2 rot;
		glm::vec2 scale;
	};

	struct RigidBody
	{
		glm::vec3 velocity;
		//Probably add weight and so on
	};

}