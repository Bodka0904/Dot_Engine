#include "GuiSlider.h"



GuiSlider::GuiSlider(const std::string& name)
	: m_text(new GuiText(name))
{
}


GuiSlider::~GuiSlider()
{
	delete m_text;
}

void GuiSlider::Init(unsigned int & VBO, unsigned int & IBO)
{
	glGenVertexArrays(1, &m_vao[0]);
	glGenVertexArrays(1, &m_vao[1]);

	glBindVertexArray(m_vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 24, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 24, (const void*)8);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);


	glBindVertexArray(m_vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 24, (const void*)(4 * sizeof(GuiVertex)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 24, (const void*)(4 * sizeof(GuiVertex) + 8));
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBindVertexArray(0);
}

void GuiSlider::Draw()
{
	glBindVertexArray(m_vao[0]);

	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glBindVertexArray(m_vao[1]);

	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void GuiSlider::SetData(glm::vec2 pos, glm::vec2 scale, glm::vec2 rot)
{
	m_rotation = rot;
	m_scale = scale;
	m_position = pos;
}

void GuiSlider::SetColor(glm::vec3 color)
{
	m_color = color;
}

void GuiSlider::UpdateData(GuiTransform & transform, glm::vec3 color)
{
	transform.SetRot(m_rotation);
	transform.SetScale(m_scale);
	transform.SetPos(glm::vec2(m_position.x - m_scale.x / 2,
		m_position.y + m_scale.y / 2));
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

glm::vec2 GuiSlider::GetPosition() const
{
	return glm::vec2();
}


glm::vec4 GuiSlider::GetCoords()
{
	return glm::vec4(m_position.x - m_scale.x / 2,
		m_position.y + m_scale.y / 2,
		m_position.x + (m_scale.x / 2 * 4),
		m_position.y - (m_scale.y / 2 * 0.2));
}
