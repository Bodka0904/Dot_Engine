#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

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