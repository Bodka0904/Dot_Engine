#include "GuiArrowButton.h"

float ArrowButton::ARROW_BUTTON_SIZE_X = 60;
float ArrowButton::ARROW_BUTTON_SIZE_Y = 40;

GuiArrowButton::GuiArrowButton(const std::string& name)
	: m_text(new GuiText(name, glm::vec2(0, -15)))
{
	m_click = new bool;
	m_click = &m_left_click;
}


GuiArrowButton::~GuiArrowButton()
{
	delete m_text;
	glDeleteVertexArrays(1, &m_vao);
}

void GuiArrowButton::Init(unsigned int & VBO, unsigned int & IBO)
{
	glGenVertexArrays(1, &m_vao);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, (const void*)8);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glBindVertexArray(0);
}

void GuiArrowButton::Draw(GuiShader & shader, GuiTransform & transform)
{
	UpdateData(transform);
	shader.Update(transform);
	shader.UpdateColor(glm::vec3(m_color, m_color, m_color));
	shader.UpdateTexOffset(glm::vec2(0, 0));

	glBindVertexArray(m_vao);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void GuiArrowButton::UpdateData(GuiTransform & transform)
{
	transform.SetScale(m_scale);
	transform.SetPos(glm::vec2(m_position.x,
		m_position.y));
}

void GuiArrowButton::SetData(glm::vec2& pos, glm::vec2& scale)
{
	m_scale = scale;
	m_position = glm::vec2(pos.x - (ArrowButton::ARROW_BUTTON_SIZE_X / 2 * m_scale.x),
		pos.y - (ArrowButton::ARROW_BUTTON_SIZE_Y / 2 * m_scale.y));

	m_text->SetData(m_position);
}

bool GuiArrowButton::MouseHoover(glm::vec2 mousePos)
{
	if (mousePos.x >= GetCoords().x && mousePos.x <= GetCoords().z
		&& mousePos.y <= GetCoords().y && mousePos.y >= GetCoords().w)
	{
		
		if (mousePos.x < GetCoords().z - ((ArrowButton::ARROW_BUTTON_SIZE_X/2 *m_scale.x)))
		{
			
			m_click = &m_left_click;
		}
		else 
		{
			m_click = &m_right_click;
		}

		return true;
	}
	else
	{
		return false;
	}
}

glm::vec4 GuiArrowButton::GetCoords()
{
	return glm::vec4(
		m_position.x,
		m_position.y + ArrowButton::ARROW_BUTTON_SIZE_Y,
		m_position.x + ArrowButton::ARROW_BUTTON_SIZE_X,
		m_position.y);
}
