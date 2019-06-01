#include "GuiButton.h"




GuiButton::GuiButton()
{
	m_data = new Button();
	
}

GuiButton::~GuiButton()
{
	delete m_data;
}

void GuiButton::Init()
{

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_data->vertices), m_data->vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &m_vbo[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_data->indices), m_data->indices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_vbo[2]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_data->colors), m_data->colors, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}


void GuiButton::Draw()
{
	
	glBindVertexArray(m_vao);

	glDrawElements(GL_TRIANGLES, sizeof(m_data->indices) / sizeof(m_data->indices[0]), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

bool GuiButton::MouseHoover(glm::vec2 mousePos)
{
	
	if (mousePos.x >= GetCoords().x && mousePos.x <= GetCoords().z
		&& mousePos.y >= GetCoords().y && mousePos.y <= GetCoords().w)
	{
		return true;
	}
	else
	{
		return false;	
	}
}


glm::vec4 GuiButton::GetCoords() const
{
	return glm::vec4(b_translation.x,
					 b_translation.y,
					 b_translation.x + m_data->scale,
					 b_translation.y + m_data->scale);
}

