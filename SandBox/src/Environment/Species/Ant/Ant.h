#pragma once
#include <glm/glm.hpp>
#include "Dot/Graphics/Transform.h"

class Ant
{
public:
	Ant(unsigned int index,const glm::vec3& position)
		:m_index(index)
	{
		m_transformation = std::make_shared<Dot::Transform>();
		m_transformation->GetPos() = position;
	}
	~Ant(){}

	void Move(const glm::vec3& pos)
	{
		m_transformation->GetPos() += pos;
		m_ModelMatrix = m_transformation->GetModel();
	}
	void Rotate(const glm::vec3& rot)
	{
		m_transformation->GetRot() += rot;
		m_ModelMatrix = m_transformation->GetModel();
	}

	const glm::mat4& GetTransformation() const { return m_ModelMatrix; }
private:
	Dot::Ref<Dot::Transform> m_transformation;
	glm::mat4 m_ModelMatrix;

	unsigned int m_index;
};
