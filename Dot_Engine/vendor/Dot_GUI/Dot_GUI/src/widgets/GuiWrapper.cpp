#include "GuiWrapper.h"


float Wrapper::WRAPPER_SIZE_X = 400.0f;
float Wrapper::WRAPPER_SIZE_Y = 400.0f;

GuiWrapper::GuiWrapper()
{
}


GuiWrapper::~GuiWrapper()
{
}

void GuiWrapper::Init(unsigned int & VBO, unsigned int & IBO)
{
	glGenVertexArrays(1, &m_vao);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 24, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 24, (const void*)8);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
}

void GuiWrapper::Draw(GuiShader & shader, GuiTransform & transform)
{

	UpdateData(transform);
	shader.Update(transform);
	shader.UpdateColor(glm::vec3(m_color, m_color, m_color));

	glBindVertexArray(m_vao);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void GuiWrapper::UpdateData(GuiTransform & transform)
{
	transform.SetScale(m_scale);
	transform.SetPos(glm::vec2(m_position.x,
		m_position.y));
}

void GuiWrapper::SetData(glm::vec2 pos, glm::vec2 scale)
{
	m_scale = scale;
	m_position = glm::vec2(pos.x - Wrapper::WRAPPER_SIZE_X/2*m_scale.x, pos.y - Wrapper::WRAPPER_SIZE_Y / 2*m_scale.y);
}

void GuiWrapper::PinToSide(glm::vec2 winSize)
{
	m_pinned = true;
	if (m_position.x > winSize.x/2 - Wrapper::WRAPPER_SIZE_X/2)
	{
		m_position.x = winSize.x - (Wrapper::WRAPPER_SIZE_X*m_scale.x);
		m_position.y = 0;

		m_scale.y = winSize.y / Wrapper::WRAPPER_SIZE_Y;
	}
	else if (m_position.x < winSize.x / 2 + Wrapper::WRAPPER_SIZE_X / 2)
	{
		m_position.x = 0;
		m_position.y = 0;

		m_scale.y = winSize.y / Wrapper::WRAPPER_SIZE_Y;
	}
	
}



bool GuiWrapper::MouseHoover(glm::vec2 mousePos)
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

glm::vec2 GuiWrapper::GetCenter() const
{
	return glm::vec2(m_position.x + (Wrapper::WRAPPER_SIZE_X/2 * m_scale.x), m_position.y + (Wrapper::WRAPPER_SIZE_Y/2 * m_scale.y));
}

glm::vec2 GuiWrapper::GetPosition() const
{
	return glm::vec2(m_position.x , m_position.y);

}


glm::vec4 GuiWrapper::GetCoords()
{
	return glm::vec4(
		m_position.x,
		m_position.y + (Wrapper::WRAPPER_SIZE_Y*m_scale.y),
		m_position.x + (Wrapper::WRAPPER_SIZE_X*m_scale.x),
		m_position.y);
}
