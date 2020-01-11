#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace Dot {
	glm::mat4 CalculateModel(const glm::vec3& pos, const glm::vec3& rot = glm::vec3(0), const glm::vec3& scale = glm::vec3(1))
	{
		glm::mat4 posMatrix = glm::translate(pos);
		glm::mat4 rotXMatrix = glm::rotate(rot.x, glm::vec3(1, 0, 0));
		glm::mat4 rotYMatrix = glm::rotate(rot.y, glm::vec3(0, 1, 0));
		glm::mat4 rotZMatrix = glm::rotate(rot.z, glm::vec3(0, 0, 1));
		glm::mat4 scaleMatrix = glm::scale(scale);

		glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;
		return posMatrix * rotMatrix * scaleMatrix;
	}
	void RotateMatrix(glm::mat4& matrix, const glm::vec3& rot)
	{
		glm::mat4 rotXMatrix = glm::rotate(rot.x, glm::vec3(1, 0, 0));
		glm::mat4 rotYMatrix = glm::rotate(rot.y, glm::vec3(0, 1, 0));
		glm::mat4 rotZMatrix = glm::rotate(rot.z, glm::vec3(0, 0, 1));
		glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

		matrix *= rotMatrix;
	}
}