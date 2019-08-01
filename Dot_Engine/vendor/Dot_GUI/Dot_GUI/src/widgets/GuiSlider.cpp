#include "GuiSlider.h"
#include "data/GuiData.h"


GuiSlider::GuiSlider(const std::string& name,const glm::vec2& size)
	: m_text(new GuiText(name, glm::vec2(5, -15))),m_size(size)
{
	std::shared_ptr<GuiVertexBuffer> m_VBO;
	std::shared_ptr<GuiIndexBuffer> m_IBO;

	m_VAO.reset(new GuiArrayBuffer());


	Slider m_data(size);
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


GuiSlider::~GuiSlider()
{
	delete m_text;
}


void GuiSlider::Draw(GuiShader& shader, GuiTransform& transform)
{
	UpdateData(transform);
	shader.Update(transform);
	shader.UpdateColor(glm::vec3(0,0,0));
	shader.UpdateTexOffset(glm::vec2(m_value,0));

	m_VAO->Bind();

	glDrawElements(GL_TRIANGLES, m_VAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);


}

void GuiSlider::SetData(glm::vec2& pos)
{
	m_position = glm::vec2(pos.x - (m_size.x / 2),
		pos.y - (m_size.y / 2));


	m_text->SetData(m_position);
}



void GuiSlider::UpdateData(GuiTransform & transform)
{
	transform.SetPos(glm::vec2(m_position.x,
		m_position.y));

}

void GuiSlider::SetValue(float value)
{
	m_value = 1 - ((value - m_position.x)/ m_size.x);
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
		m_position.y + m_size.y,
		m_position.x + m_size.x,
		m_position.y);
}
