#include "stdafx.h"
#include "Dropdown.h"


namespace Dot {
	Dropdown::Dropdown(const std::string& label, const glm::vec2& position, const glm::vec2& size, float labelsize)
	{
		glm::vec2 texCoords[4] = {
					  glm::vec2(0,0),
					  glm::vec2(0.25,0),
					  glm::vec2(0.25,0.25),
					  glm::vec2(0,0.25)
		};


		glm::vec2 offset = glm::vec2(5, 2);
		Dropbox box;
		box.Clicked = false;
		box.Position = glm::vec2(position.x, position.y);
		box.Size = size;
		box.Label = std::make_shared<Text>("Arial", label, box.Position + offset, glm::vec2(labelsize, labelsize), MAX_CHAR_PER_LABEL);
		box.Index = Gui::Get()->PopIndex();
		box.Quad = QuadVertex(position, size, &texCoords[0]);
		m_Box.push_back(box);


		Gui::Get()->UpdateLabelBuffer(m_Box[0].Index, m_Box[0].Label->GetVertice(0), m_Box[0].Label->GetNumChar());
		Gui::Get()->UpdateVertexBuffer(m_Box[0].Index, &m_Box[0].Quad);
	}
	Dropdown::~Dropdown()
	{
		for (int i = 0; i < m_Box.size();++i)
			Gui::Get()->PushIndex(m_Box[i].Index);
	}
	bool Dropdown::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = getCoords();
		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			if (m_Clicked)
			{
				glm::vec2 size(m_Box[0].Size.x, m_Box[0].Size.y + m_Box.size() * (m_Box[0].Size.y + s_BoxOffset));
				float pos = (mousePos.y - m_Box[0].Position.y - m_Box[0].Size.y - s_BoxOffset) / m_Box[0].Size.y ;
				int index = (int)std::floor(pos)+1;
				if (index < 1 || index >= m_Box.size())
				{
					return true;
					m_ClickedIndex = 0;
				}
				else
				{
					m_Box[m_ClickedIndex].Clicked = false;
					m_Box[m_ClickedIndex].Size.x = m_Box[0].Size.x;
					m_Box[m_ClickedIndex].Quad.SetPosition(m_Box[m_ClickedIndex].Position, m_Box[m_ClickedIndex].Size);
					
					Gui::Get()->UpdateVertexBuffer(m_Box[m_ClickedIndex].Index, &m_Box[m_ClickedIndex].Quad);
					if (m_ClickedIndex != index)
					{
						m_Box[index].Clicked = true;
						m_Box[index].Size.x *= 1.2;
						m_Box[index].Quad.SetPosition(m_Box[index].Position, m_Box[index].Size);
						Gui::Get()->UpdateVertexBuffer(m_Box[index].Index, &m_Box[index].Quad);
						m_ClickedIndex = index;
					}
					else
						m_ClickedIndex = 0;

					return false;
				}
			}
			return true;
		}
		return false;
	}
	void Dropdown::ClickHandle()
	{
		m_Clicked = !m_Clicked;
		glm::vec2 offset = glm::vec2(5, 2);
		if (m_Clicked)
		{
			for (int i = 1; i < m_Box.size();++i)
			{
				m_Box[i].Quad.SetPosition(m_Box[i].Position, m_Box[i].Size);			
				Gui::Get()->UpdateVertexBuffer(m_Box[i].Index, &m_Box[i].Quad);
				m_Box[i].Label->SetPosition(m_Box[i].Position + offset);
				Gui::Get()->UpdateLabelBuffer(m_Box[i].Index, m_Box[i].Label->GetVertice(0), m_Box[i].Label->GetNumChar());
			}
		}
		else
		{
			for (int i = 1; i < m_Box.size();++i)
			{		
				m_Box[i].Quad.SetPosition(glm::vec2(0), glm::vec2(0));
				Gui::Get()->UpdateVertexBuffer(m_Box[i].Index, &m_Box[i].Quad);
				m_Box[i].Label->SetPosition(glm::vec2(-100, -100));
				Gui::Get()->UpdateLabelBuffer(m_Box[i].Index, m_Box[i].Label->GetVertice(0), m_Box[i].Label->GetNumChar());					
			}
		
		}
	}
	void Dropdown::Minimize()
	{
		m_Box[0].Quad.SetPosition(glm::vec2(0), glm::vec2(0));
		Gui::Get()->UpdateVertexBuffer(m_Box[0].Index, &m_Box[0].Quad);

		m_Box[0].Label->SetPosition(glm::vec2(-100, -100));
		Gui::Get()->UpdateLabelBuffer(m_Box[0].Index, m_Box[0].Label->GetVertice(0), m_Box[0].Label->GetNumChar());


		for (int i = 1; i < m_Box.size();++i)
		{
			m_Box[i].Quad.SetPosition(glm::vec2(0), glm::vec2(0));
			m_Box[i].Label->SetPosition(glm::vec2(-100, -100));

			Gui::Get()->UpdateVertexBuffer(m_Box[i].Index, &m_Box[i].Quad);
			Gui::Get()->UpdateLabelBuffer(m_Box[i].Index, m_Box[i].Label->GetVertice(0), m_Box[i].Label->GetNumChar());
		}
	}
	void Dropdown::Move(const glm::vec2 pos)
	{
		glm::vec2 offset(5, 2);

		m_Box[0].Position += pos;
		m_Box[0].Quad.SetPosition(m_Box[0].Position, m_Box[0].Size);
		Gui::Get()->UpdateVertexBuffer(m_Box[0].Index, &m_Box[0].Quad);
		
		m_Box[0].Label->SetPosition(glm::vec2(m_Box[0].Position.x, m_Box[0].Position.y) + offset);
		Gui::Get()->UpdateLabelBuffer(m_Box[0].Index, m_Box[0].Label->GetVertice(0), m_Box[0].Label->GetNumChar());

		if (m_Clicked)
		{	
			for (int i = 1;i < m_Box.size();++i)
			{
				m_Box[i].Position += pos;
				m_Box[i].Quad.SetPosition(m_Box[i].Position, m_Box[i].Size);
				m_Box[i].Label->SetPosition(m_Box[i].Position + offset);

				Gui::Get()->UpdateVertexBuffer(m_Box[i].Index, &m_Box[i].Quad);
				Gui::Get()->UpdateLabelBuffer(m_Box[i].Index, m_Box[i].Label->GetVertice(0), m_Box[i].Label->GetNumChar());
			}
	
		}
		else
		{
			for (int i = 1;i < m_Box.size();++i)
			{
				m_Box[i].Position += pos;
				m_Box[i].Quad.Move(pos);
				m_Box[i].Label->SetPosition(m_Box[i].Position + offset);
			}
		
		}
	}
	void Dropdown::SetPosition(const glm::vec2& pos)
	{
		glm::vec2 offset = glm::vec2(5, 2);

		m_Box[0].Position = pos;
		m_Box[0].Quad.SetPosition(pos, m_Box[0].Size);
		Gui::Get()->UpdateVertexBuffer(m_Box[0].Index, &m_Box[0].Quad);
		
		m_Box[0].Label->SetPosition(glm::vec2(m_Box[0].Position.x, m_Box[0].Position.y) + offset);
		Gui::Get()->UpdateLabelBuffer(m_Box[0].Index, m_Box[0].Label->GetVertice(0), m_Box[0].Label->GetNumChar());

		if (m_Clicked)
		{		
			for (int i = 1;i < m_Box.size();++i)
			{
				m_Box[i].Position = pos;
				m_Box[i].Quad.SetPosition(m_Box[i].Position, m_Box[i].Size);
				m_Box[i].Label->SetPosition(m_Box[i].Position + offset);

				Gui::Get()->UpdateVertexBuffer(m_Box[i].Index, &m_Box[i].Quad);
				Gui::Get()->UpdateLabelBuffer(m_Box[i].Index, m_Box[i].Label->GetVertice(0), m_Box[i].Label->GetNumChar());
			}
	
		}
		else
		{
			for (int i = 1;i < m_Box.size();++i)
			{
				m_Box[i].Position = pos;
				m_Box[i].Quad.SetPosition(m_Box[i].Position, m_Box[i].Size);
				m_Box[i].Label->SetPosition(m_Box[i].Position + offset);
			}
		}
	}
	const glm::vec2& Dropdown::GetLabelSize()
	{
		return m_Box[0].Label->GetSize();
	}
	void Dropdown::AddDropbox(const std::string& label, float labelsize)
	{
		glm::vec2 offset = glm::vec2(5, 0);
		Dropbox box;
		box.Clicked = false;
		box.Position = glm::vec2(m_Box[0].Position.x, m_Box[0].Position.y + m_Box.size() * (m_Box[0].Size.y + s_BoxOffset));
		box.Size = m_Box[0].Size;
		box.Label = std::make_shared<Text>("Arial", label, box.Position + offset, glm::vec2(labelsize, labelsize), MAX_CHAR_PER_LABEL);
		box.Index = Gui::Get()->PopIndex();
		box.Quad = m_Box[0].Quad;
		m_Box.push_back(box);
	}
	const bool Dropdown::GetBoxClicked(int index)
	{
		D_ASSERT(index < m_Box.size(), "Index out of range");
		if (m_Box[index].Clicked)
		{
			m_Box[index].Clicked = false;
			return true;
		}
		return false;
	}
	const bool Dropdown::GetClicked()
	{
		return m_Clicked;
	}
	Dropdown& Dropdown::Get(const std::string& label)
	{
		Dropdown& button = (Dropdown&)Gui::Get()->GetWidget(label);
		return button;
	}
	Dropdown& Dropdown::GetWrapped(const std::string& wrapper, const std::string& label)
	{
		Dropdown& button = (Dropdown&)Gui::Get()->GetWrappedWidget(wrapper, label);
		return button;
	}
	void Dropdown::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size, float labelsize)
	{
		if (Gui::Get())
		{
			D_ASSERT(label.size() < MAX_CHAR_PER_LABEL, "Max len of label is %d", MAX_CHAR_PER_LABEL);
			Ref<Dropdown> button = std::make_shared<Dropdown>(label, position, size, labelsize);
			Gui::Get()->AddWidget(label, button);
		}
	}
	glm::vec4 Dropdown::getCoords()
	{
		if (m_Clicked)
		{
			glm::vec2 size(m_Box[0].Size.x, m_Box.size() * (m_Box[0].Size.y + s_BoxOffset));

			return glm::vec4(m_Box[0].Position.x,
				m_Box[0].Position.y + size.y,
				m_Box[0].Position.x + size.x,
				m_Box[0].Position.y);
		}
		return glm::vec4(m_Box[0].Position.x,
			m_Box[0].Position.y + m_Box[0].Size.y,
			m_Box[0].Position.x + m_Box[0].Size.x,
			m_Box[0].Position.y);
	}
}