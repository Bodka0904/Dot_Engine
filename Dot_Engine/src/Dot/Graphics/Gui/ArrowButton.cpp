#include "stdafx.h"
#include "ArrowButton.h"

#include <GL/glew.h>

namespace Dot {
	ArrowButton::ArrowButton(const std::string& label, const glm::vec2& position, const glm::vec2& size)
		:
		Widget(),
		m_Size(size)
	{
		m_Clicked = &m_LeftClick;

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
	ArrowButton::~ArrowButton()
	{
		
	}
	void ArrowButton::Update(const Ref<Shader>& shader)
	{
		shader->UploadUniformFloat("u_TexOffset", 0);
		*m_Clicked = false;
	}
	void ArrowButton::RenderLabel()
	{
		m_Label->PrintText("ArialBolt");
	}
	void ArrowButton::SetPosition(const glm::vec2& pos)
	{
		m_Transform.GetPos() = pos;
	
		glm::vec2 newPos[4] = {
			glm::vec2(m_Transform.GetPos()),
			glm::vec2(m_Transform.GetPos().x + m_Size.x,m_Transform.GetPos().y),
			glm::vec2(m_Transform.GetPos() + m_Size),
			glm::vec2(m_Transform.GetPos().x,m_Transform.GetPos().y + m_Size.y)
		};
		WidgetStack::UpdatePosBuffer(m_Index, sizeof(glm::vec2) * 4, (void*)& newPos[0]);
	}
	bool ArrowButton::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = GetCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			if (mousePos.x < coords.z - m_Size.x / 2)
			{
				m_Clicked = &m_LeftClick;
			}
			else
			{
				m_Clicked = &m_RightClick;
			}
			return true;
		}

		return false;
	}
	void ArrowButton::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size)
	{
		glm::vec2 texcoords[4] = {
			   glm::vec2(0.5,0),
			   glm::vec2(1.0,0),
			   glm::vec2(1.0,0.25),
			   glm::vec2(0.5,0.25)
		};
		Quad quad(glm::vec2(0, 0), size);
		Ref<ArrowButton> button;
		button = std::make_shared<ArrowButton>(label, position, size);

		WidgetStack::AddWidget(label, button,quad, &texcoords[0]);
	}
	glm::vec4 ArrowButton::GetCoords()
	{
		return glm::vec4(m_Transform.GetPos().x,
			m_Transform.GetPos().y + m_Size.y,
			m_Transform.GetPos().x + m_Size.x,
			m_Transform.GetPos().y);
	}
}