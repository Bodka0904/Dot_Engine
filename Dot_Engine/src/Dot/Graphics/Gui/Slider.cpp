#include "stdafx.h"
#include "Slider.h"

#include <GL/glew.h>

namespace Dot {
	Slider::Slider(const std::string& label, const glm::vec2& position, const glm::vec2& size, float* value)
		:
		Widget(),
		m_Size(size)
	{
		m_Value = value;

		m_Transform.SetPos(position);
		


		float letterSize = size.x / label.length()/3;
		if (letterSize >= 40)
		{
			letterSize = 40;
		}
		else if (letterSize <= 15)
		{
			letterSize = 15;
		}
		float offsetx = size.x - (label.length() * letterSize);
		m_Label = std::make_shared<Text>(label, offsetx / 2, -size.y, letterSize, size.y);

	}
	Slider::~Slider()
	{
	}
	void Slider::Update(const Ref<Shader>& shader)
	{
		
	}
	void Slider::RenderLabel()
	{
		m_Label->PrintText("ArialBolt");
	}
	void Slider::SetPosition(const glm::vec2& pos)
	{
		m_Transform.GetPos() = pos;
	
		glm::vec2 newPos[4] = {
			glm::vec2(m_Transform.GetPos()),
			glm::vec2(m_Transform.GetPos().x + m_Size.x,m_Transform.GetPos().y),
			glm::vec2(m_Transform.GetPos() + m_Size),
			glm::vec2(m_Transform.GetPos().x,m_Transform.GetPos().y + m_Size.y)
		};
		Gui::UpdatePosBuffer(m_Index, sizeof(glm::vec2) * 4, (void*)& newPos[0]);
	}
	void Slider::ClickHandle()
	{
		*m_Value = m_TempStorage;
		if (*m_Value >= 0.96f)
		{
			m_TexOffset = -*m_Value - 0.1;
			glm::vec2 texcoords[4] = {
					glm::vec2(0.5+ m_TexOffset/2,0.5),
					glm::vec2(1 +  m_TexOffset/2,0.5),
					glm::vec2(1 +  m_TexOffset/2,0.75),
					glm::vec2(0.5 + m_TexOffset/2,0.75)
			};
			Gui::UpdateTexBuffer(m_Index, sizeof(Quad), &texcoords[0]);
		}
		else if (*m_Value <= 0.02f)
		{
			m_TexOffset = -*m_Value + 0.1;
			glm::vec2 texcoords[4] = {
					glm::vec2(0.5 + m_TexOffset / 2,0.5),
					glm::vec2(1 + m_TexOffset / 2,0.5),
					glm::vec2(1 + m_TexOffset / 2,0.75),
					glm::vec2(0.5 + m_TexOffset / 2,0.75)
			};
			Gui::UpdateTexBuffer(m_Index, sizeof(Quad), &texcoords[0]);
		}
		else
		{
			m_TexOffset = -*m_Value;
			glm::vec2 texcoords[4] = {
					glm::vec2(0.5 + m_TexOffset / 2,0.5),
					glm::vec2(1 + m_TexOffset / 2,0.5),
					glm::vec2(1 + m_TexOffset / 2,0.75),
					glm::vec2(0.5 + m_TexOffset / 2,0.75)
			};
			Gui::UpdateTexBuffer(m_Index, sizeof(Quad), &texcoords[0]);
		}
	}
	bool Slider::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = GetCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			m_TempStorage = ((mousePos.x - m_Transform.GetPos().x) / m_Size.x);
			if (m_TempStorage <= 0.02f)
			{
				m_TempStorage = 0.0f;
			}
			else if (m_TempStorage >= 0.96f)
			{
				m_TempStorage = 1.0f;
			}
			
			return true;
		}

		return false;
	}
	void Slider::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size,float*value)
	{
		glm::vec2 texcoords[4] = {
			   glm::vec2(0.5,0.5),
			   glm::vec2(1,0.5),
			   glm::vec2(1,0.75),
			   glm::vec2(0.5,0.75)
		};
		Quad quad(glm::vec2(0, 0), size);
		Ref<Slider> slider;
		slider = std::make_shared<Slider>(label, position, size,value);

		Gui::AddWidget(label, slider,quad, &texcoords[0]);
	}
	glm::vec4 Slider::GetCoords()
	{
		return glm::vec4(m_Transform.GetPos().x,
			m_Transform.GetPos().y + m_Size.y,
			m_Transform.GetPos().x + m_Size.x,
			m_Transform.GetPos().y);
	}
}