#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class GuiTransform
{
public:

	GuiTransform(const glm::vec2& pos = glm::vec2(), const glm::vec2& scale = glm::vec2(1.0f, 1.0f))
		:
		m_pos(pos),
		m_scale(scale)
	{
	}

	inline glm::mat4 GetOrtho() const
	{
	
		glm::mat4 ortho = glm::ortho(0.0f, m_win_size.x, m_win_size.y, 0.0f, -5.0f, 5.0f);
	
		return ortho;
	}

	
	
	inline glm::vec2& GetPos() { return m_pos; }
	inline glm::vec2& GetScale() { return m_scale; }

	inline void SetOrtho(glm::vec2 win_size) { m_win_size = win_size; }
	inline void SetPos(const glm::vec2& pos) { m_pos = pos; }
	inline void SetScale(const glm::vec2& scale) { m_scale = scale; }



private:
	glm::vec2 m_pos;
	glm::vec2 m_scale;

	glm::vec2 m_win_size;

};


