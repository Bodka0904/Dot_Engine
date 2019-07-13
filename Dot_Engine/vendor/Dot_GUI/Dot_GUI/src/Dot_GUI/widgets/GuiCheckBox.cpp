#include "GuiCheckBox.h"


float CheckBox::CHECKBOX_SIZE_X = 40;
float CheckBox::CHECKBOX_SIZE_Y = 40;

GuiCheckBox::GuiCheckBox(const std::string& name)
	: m_text(new GuiText(name, glm::vec2(0, -15)))
{
}


GuiCheckBox::~GuiCheckBox()
{
	glDeleteVertexArrays(1, &m_vao);
	delete m_text;

}

void GuiCheckBox::Init(unsigned int& VBO, unsigned int& IBO)
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

void GuiCheckBox::Draw(GuiShader& shader, GuiTransform& transform)
{	
	UpdateData(transform);
	shader.Update(transform);
	if (checked)
	{
		shader.UpdateTexOffset(glm::vec2(0.5,0));	
	}
	else
	{
		shader.UpdateTexOffset(glm::vec2(0,0));
	}
	shader.UpdateColor(glm::vec3(0, 0, 0));

	glBindVertexArray(m_vao);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

}

void GuiCheckBox::UpdateData(GuiTransform & transform)
{
	
	transform.SetScale(m_scale);
	transform.SetPos(glm::vec2(m_position.x,
		m_position.y));
}

void GuiCheckBox::SetData(glm::vec2 pos, glm::vec2 scale)
{
	m_scale = scale;
	m_position = glm::vec2(pos.x - (CheckBox::CHECKBOX_SIZE_X / 2 * m_scale.x),
		pos.y - (CheckBox::CHECKBOX_SIZE_Y / 2 * m_scale.y));

	m_text->SetData(m_position);
}



bool GuiCheckBox::MouseHoover(glm::vec2 mousePos)
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



glm::vec4 GuiCheckBox::GetCoords()
{
	return glm::vec4(
		m_position.x,
		m_position.y + CheckBox::CHECKBOX_SIZE_Y,
		m_position.x + CheckBox::CHECKBOX_SIZE_X,
		m_position.y);
}
