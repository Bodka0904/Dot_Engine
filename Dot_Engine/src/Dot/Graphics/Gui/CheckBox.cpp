#include "stdafx.h"
#include "CheckBox.h"

#include <GL/glew.h>

namespace Dot {
	CheckBox::CheckBox(const std::string& label, const glm::vec2& position, const glm::vec2& size)
		:
		Widget(),
		m_Size(size)
	{

		m_Transform.SetPos(position);

		float letterSize = size.x / label.length();
		if (letterSize >= 40)
		{
			letterSize = 40;
		}
		else if (letterSize <= 15)
		{
			letterSize = 15;
		}
		float offsetx = size.x - (label.length() * letterSize);
		m_Label = std::make_shared<Text>(label, offsetx / 2, -size.y / 2, letterSize, size.y / 2);

	}
	CheckBox::~CheckBox()
	{
	}

	void CheckBox::Update(const Ref<Shader>& shader)
	{
		shader->UploadUniformFloat("u_TexOffset", (float)m_TexOffset);
	}
	void CheckBox::RenderLabel()
	{
		m_Label->PrintText("ArialBolt");
	}
	void CheckBox::SetPosition(const glm::vec2& pos)
	{
		m_Transform.GetPos() = pos;
		m_Transform.UpdateModel();
		glm::vec2 newPos[4] = { m_Transform.GetPos() ,m_Transform.GetPos() ,m_Transform.GetPos() ,m_Transform.GetPos() };

		WidgetStack::UpdateTransfBuffer(m_Index, sizeof(glm::vec2) * 4, (void*)& newPos[0]);
	}
	bool CheckBox::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = GetCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			return true;
		}

		return false;
	}
	void CheckBox::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size)
	{
		glm::vec2 texcoords[4] = {
			   glm::vec2(0,0.25),
			   glm::vec2(0.5,0.25),
			   glm::vec2(0.5,0.5),
			   glm::vec2(0,0.5)
		};
		Quad quad(glm::vec2(0, 0), size, &texcoords[0]);
		Ref<CheckBox> checkbox;
		checkbox = std::make_shared<CheckBox>(label, position, size);

		WidgetStack::AddWidget(label, checkbox, quad);
	}
	glm::vec4 CheckBox::GetCoords()
	{
		return glm::vec4(m_Transform.GetPos().x,
			m_Transform.GetPos().y + m_Size.y,
			m_Transform.GetPos().x + m_Size.x,
			m_Transform.GetPos().y);
	}
}