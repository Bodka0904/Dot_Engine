#include "GuiSlider.h"


float Slider::SLIDER_SIZE_X = 150;
float Slider::SLIDER_SIZE_Y = 20;

GuiSlider::GuiSlider(const std::string& name)
	: m_text(new GuiText(name, glm::vec2(5, -15)))
{
}


GuiSlider::~GuiSlider()
{
	glDeleteVertexArrays(1, &m_vao);
	delete m_text;
}

void GuiSlider::Init(unsigned int & VBO, unsigned int & IBO)
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

void GuiSlider::Draw(GuiShader& shader, GuiTransform& transform)
{
	UpdateData(transform);
	shader.Update(transform);
	shader.UpdateColor(glm::vec3(0,0,0));
	shader.UpdateTexOffset(glm::vec2(m_value,0));

	glBindVertexArray(m_vao);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);


}

void GuiSlider::SetData(glm::vec2 pos, glm::vec2 scale)
{
	m_scale = scale;
	m_position = glm::vec2(pos.x - (Slider::SLIDER_SIZE_X / 2 * m_scale.x),
		pos.y - (Slider::SLIDER_SIZE_Y / 2 * m_scale.y));

	m_text->SetData(m_position);
}



void GuiSlider::UpdateData(GuiTransform & transform)
{
	transform.SetScale(m_scale);
	transform.SetPos(glm::vec2(m_position.x,
		m_position.y));

}

void GuiSlider::SetValue(float value)
{
	m_value = 1 - ((value - m_position.x)/ (Slider::SLIDER_SIZE_X + m_scale.x));
	if (m_value <= 0.02f)
	{
		m_value = 0.0f;
	}
}

bool GuiSlider::MouseHoover(glm::vec2 mousePos)
{
	if (mousePos.x >= GetCoords().x && mousePos.x <= GetCoords().z
		&& mousePos.y <= GetCoords().y && mousePos.y >= GetCoords().w)
	{
		return true;

	}
	else
	{
		return false;
	}
}



glm::vec4 GuiSlider::GetCoords()
{
	return glm::vec4(
		m_position.x,
		m_position.y + Slider::SLIDER_SIZE_Y,
		m_position.x + Slider::SLIDER_SIZE_X,
		m_position.y);
}
