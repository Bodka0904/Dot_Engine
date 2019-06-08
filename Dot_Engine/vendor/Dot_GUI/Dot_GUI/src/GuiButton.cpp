#include "GuiButton.h"
#include "stdafx.h"

int GuiButton::attachedButton = -1;


GuiButton::GuiButton(const std::string& name,glm::vec3 color)
	: m_data(color),m_text(new GuiText(name))
{

}

GuiButton::~GuiButton()
{
	delete m_text;
}

void GuiButton::Init()
{

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_data.vertices), m_data.vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &m_vbo[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_data.indices), m_data.indices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_vbo[2]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_data.colors), m_data.colors, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);


	glBindVertexArray(0);

}


void GuiButton::Draw()
{
	
	
	glBindVertexArray(m_vao);

	glDrawElements(GL_TRIANGLES, sizeof(m_data.indices) / sizeof(m_data.indices[0]), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void GuiButton::UpdateData(GuiTransform& transform)
{
	transform.SetRot(m_rotation);
	transform.SetScale(m_scale);
	transform.SetPos(glm::vec2(m_position.x - m_scale.x/2,
							   m_position.y + m_scale.y/2));
}

void GuiButton::SetData(glm::vec2 pos,glm::vec2 scale,glm::vec2 rot)
{
	m_rotation = rot;
	m_scale = scale;
	m_position = pos;
}



bool GuiButton::MouseHoover(glm::vec2 mousePos)
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


glm::vec4 GuiButton::GetCoords() 
{
	return glm::vec4(m_position.x - m_scale.x/2,
					 m_position.y + m_scale.y/2,
					 m_position.x + m_scale.x,
					 m_position.y - m_scale.y);
}

