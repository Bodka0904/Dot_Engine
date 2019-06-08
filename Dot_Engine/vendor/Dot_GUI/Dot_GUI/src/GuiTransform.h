#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class GuiTransform
{
public:

	GuiTransform(const glm::vec2& pos = glm::vec2(), const glm::vec2& rot = glm::vec2(), const glm::vec2& scale = glm::vec2(1.0f, 1.0f))
		:
		m_pos(pos),
		m_rot(rot),
		m_scale(scale)
	{
	}

	inline glm::mat4 GetModel() const
	{
		glm::mat4 posMatrix = glm::translate(glm::vec3(m_pos,0));
		glm::mat4 rotXMatrix = glm::rotate(m_rot.x, glm::vec3(1, 0, 0));
		glm::mat4 rotYMatrix = glm::rotate(m_rot.y, glm::vec3(0, 1, 0));
		glm::mat4 scaleMatrix = glm::scale(glm::vec3(m_scale,0));

		glm::mat4 rotMatrix = rotYMatrix * rotXMatrix;

		return posMatrix * rotMatrix *scaleMatrix;
	}
	inline glm::vec2& GetPos() { return m_pos; }
	inline glm::vec2& GetRot() { return m_rot; }
	inline glm::vec2& GetScale() { return m_scale; }

	inline void SetPos(const glm::vec2& pos) { m_pos = pos; }
	inline void SetRot(const glm::vec2& rot) { m_rot = rot; }
	inline void SetScale(const glm::vec2& scale) { m_scale = scale; }



private:
	glm::vec2 m_pos;
	glm::vec2 m_scale;
	glm::vec2 m_rot;

};


