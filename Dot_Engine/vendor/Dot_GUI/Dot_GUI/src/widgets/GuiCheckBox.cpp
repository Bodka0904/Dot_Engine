#include "GuiCheckBox.h"
#include "data/GuiData.h"
#include "graphics/GuiBuffer.h"
#include <GL/glew.h>



GuiCheckBox::GuiCheckBox(const std::string& name, const glm::vec2& size)
	: m_text(new GuiText(name, glm::vec2(0, -15))),m_size(size)
{
	std::shared_ptr<GuiVertexBuffer> m_VBO;
	std::shared_ptr<GuiIndexBuffer> m_IBO;

	m_VAO.reset(new GuiArrayBuffer());


	CheckBox m_data(size);
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


GuiCheckBox::~GuiCheckBox()
{
	delete m_text;
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

	m_VAO->Bind();

	glDrawElements(GL_TRIANGLES, m_VAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

}

void GuiCheckBox::UpdateData(GuiTransform & transform)
{
	
	transform.SetPos(glm::vec2(m_position.x,
		m_position.y));
}

void GuiCheckBox::SetData(const glm::vec2& pos)
{
	m_position = glm::vec2(pos.x - (m_size.x / 2),
		pos.y - (m_size.y / 2));

	m_text->SetData(m_position);
}



bool GuiCheckBox::MouseHoover(const glm::vec2& mousePos)
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
		m_position.y + m_size.y,
		m_position.x + m_size.x,
		m_position.y);
}
