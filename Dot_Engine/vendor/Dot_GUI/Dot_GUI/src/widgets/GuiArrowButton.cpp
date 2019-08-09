#include "GuiArrowButton.h"
#include "graphics/GuiBuffer.h"
#include "data/GuiData.h"
#include <GL/glew.h>


GuiArrowButton::GuiArrowButton(const std::string& name, const glm::vec2& size)
	: m_text(new GuiText(name, glm::vec2(0, -15))),m_size(size)
{
	m_click = new bool;
	m_click = &m_left_click;

	std::shared_ptr<GuiVertexBuffer> m_VBO;
	std::shared_ptr<GuiIndexBuffer> m_IBO;

	m_VAO.reset(new GuiArrayBuffer());


	ArrowButton m_data(size);
	GuiBufferLayout layout = {
		{0,GuiShaderDataType::Float2, "position" },
		{1,GuiShaderDataType::Float2, "texCoords" }
	};

	m_VBO.reset(new GuiVertexBuffer(&m_data.m_vertices[0], 4 * sizeof(GuiVertex)));
	m_VBO->SetLayout(layout);
	m_VAO->AddVBO(m_VBO);

	m_IBO.reset(new GuiIndexBuffer((unsigned int*)& m_data.indices[0], 6));
	m_VAO->AddIBO(m_IBO);
}


GuiArrowButton::~GuiArrowButton()
{
	
	delete m_text;
	
}


void GuiArrowButton::Draw(GuiShader & shader, GuiTransform & transform)
{
	UpdateData(transform);
	shader.Update(transform);
	shader.UpdateColor(glm::vec3(m_color, m_color, m_color));
	shader.UpdateTexOffset(glm::vec2(0, 0));

	m_VAO->Bind();

	glDrawElements(GL_TRIANGLES, m_VAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void GuiArrowButton::UpdateData(GuiTransform & transform)
{
	transform.SetPos(glm::vec2(m_position.x,
		m_position.y));
}

void GuiArrowButton::SetData(const glm::vec2& pos)
{
	m_position = glm::vec2(pos.x - (m_size.x / 2),
		pos.y - (m_size.y / 2));

	m_text->SetData(m_position);
}

bool GuiArrowButton::MouseHoover(const glm::vec2& mousePos)
{
	if (mousePos.x >= GetCoords().x && mousePos.x <= GetCoords().z
		&& mousePos.y <= GetCoords().y && mousePos.y >= GetCoords().w)
	{
		
		if (mousePos.x < GetCoords().z - (m_size.x/2))
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
		m_position.y + m_size.y,
		m_position.x + m_size.x,
		m_position.y);
}
