#include "GuiButton.h"


float Button::BUTTON_SIZE_X = 60;
float Button::BUTTON_SIZE_Y = 40;

GuiButton::GuiButton(const std::string& name)
	: m_text(new GuiText(name))
{

}

GuiButton::~GuiButton()
{
	delete m_text;
	
}

void GuiButton::Init(unsigned int& VBO, unsigned int& IBO)
{

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 24, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 24,(const void*)8);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	
	glBindVertexArray(0);

}


void GuiButton::Draw(GuiShader& shader, GuiTransform& transform)
{
	UpdateData(transform);
	shader.Update(transform);
	shader.UpdateColor(glm::vec3(m_color, m_color, m_color));
	
	glBindVertexArray(m_vao);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	
}

void GuiButton::UpdateData(GuiTransform& transform)
{

	transform.SetScale(m_scale);
	transform.SetPos(m_position);
}

void GuiButton::SetData(glm::vec2 pos,glm::vec2 scale)
{
	m_scale = scale;
	m_position = glm::vec2(pos.x - (Button::BUTTON_SIZE_X /2 * m_scale.x),
		pos.y - (Button::BUTTON_SIZE_Y / 2 * m_scale.y));

	m_text->SetData(m_position);
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
	return glm::vec4(m_position.x,
					 m_position.y + Button::BUTTON_SIZE_Y,
					 m_position.x + Button::BUTTON_SIZE_X,
					 m_position.y);
}

