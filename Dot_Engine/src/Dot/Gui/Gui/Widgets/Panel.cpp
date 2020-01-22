#include "stdafx.h"
#include "Panel.h"
#include "Dot/Gui/Gui/GuiApplication.h"

namespace Dot {
	Dot::Panel::Panel(const glm::vec2& position, const glm::vec2& size, const glm::vec3& labelColor, const std::string& label, ElementPosition pos)
		:
		m_Label("Arial", label, glm::vec2(position.x, position.y + 4), glm::vec2(0.2f), labelColor, MAX_CHAR_PER_LABEL),
		m_Position(position),
		m_Size(size),
		m_MaxSize(size),
		m_Pos(pos)
	{
		yOffset = 2*m_Label.GetSize().y;

		glm::vec2 panelCoords[4] = {
					   glm::vec2(0.0f ,		  1.0f / 8.0f),
					   glm::vec2(1.0f / 8.0f, 1.0f / 8.0f),
					   glm::vec2(1.0f / 8.0f, 2.0f / 8.0f),
					   glm::vec2(0.0f ,		  2.0f / 8.0f)
		};

		m_PanelSize = glm::vec2(size.x, Font::GetFont("Arial")->GetData().lineHeight * 0.2f + 5);
		m_PanelQuad = QuadVertex2D(glm::vec2(position),m_PanelSize , glm::vec3(0.5, 0.5, 0.5), &panelCoords[0]);
		m_Quad = QuadVertex2D(position, size, glm::vec3(1, 1, 1), &panelCoords[0]);

		m_Index = GuiApplication::Get()->PopIndex();
		m_PanelIndex = GuiApplication::Get()->PopIndex();


		GuiApplication::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
		GuiApplication::Get()->UpdateVertexBuffer(m_PanelIndex, &m_PanelQuad);
		GuiApplication::Get()->UpdateLabelBuffer(m_Index, m_Label.GetVertice(0), m_Label.GetNumChar());
	}

	Dot::Panel::~Panel()
	{
		GuiApplication::Get()->PushIndex(m_PanelIndex);
		GuiApplication::Get()->PushIndex(m_Index);
	}

	void Panel::Move(const glm::vec2& pos)
	{
		m_Position += pos;
		
		updateBuffers();

		for (auto& widg : m_Widget)
		{
			widg.second->Move(pos);
		}
	}

	void Panel::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;
		
		updateBuffers();

		for (auto& widg : m_Widget)
		{
			widg.second->SetPosition(pos);
		}
	}

	void Panel::SetColor(const glm::vec3& color)
	{
		m_Quad.SetColor(color);
		updateBuffers();
	}

	void Panel::SetSize(const glm::vec2& size)
	{
		m_Size = size;
		
		m_PanelQuad.SetPosition(m_Position, glm::vec2(m_Size.x, Font::GetFont("Arial")->GetData().lineHeight * 0.2f));
		m_Quad.SetPosition(m_Position, m_Size);
		m_Label.SetPosition(glm::vec2(m_Position.x + m_Size.x, m_Size.y));
		updateBuffers();
	}

	void Panel::AddWidget(const std::string& name, Ref<Widget> widget)
	{
		D_ASSERT(m_Widget.find(name) == m_Widget.end(), "Widget with name %s already exists",name.c_str());
		if (widget->GetSize().y > rowSize)
			rowSize = widget->GetSize().y;
		
		if (m_Position.x + xOffset + widget->GetSize().x > m_Position.x + m_Size.x)
		{
			xOffset = 10;
			yOffset += rowSize;
			rowCount = 0;
		}
		if (m_Position.y + yOffset + widget->GetSize().y > m_Position.y + m_Size.y)
		{
			widget->StopRender();
		}
		else
		{
			widget->SetPosition(glm::vec2(m_Position.x + xOffset, m_Position.y + yOffset));
			xOffset += widget->GetSize().x + 10;
			rowCount++;
		}
		m_Widget[name] = widget;
	}

	void Panel::Update(const glm::vec2& mousePos)
	{
		if (m_Clicked)
		{
			switch (m_Pos)
			{
			case ElementPosition::LEFT:
				m_Size.x = abs(mousePos.x - m_Position.x);
				if (m_Position.x + m_Size.x > Input::GetWindowSize().x/2)
					m_Size.x = Input::GetWindowSize().x/2;
				break;
			case ElementPosition::RIGHT:
				m_Size.x += m_Position.x - mousePos.x;
				m_Position.x = mousePos.x;
				if (m_Position.x < Input::GetWindowSize().x / 2)
				{
					m_Position.x = Input::GetWindowSize().x / 2;
					m_Size.x = Input::GetWindowSize().x / 2;
				}
				break;
			case ElementPosition::BOTTOM:
				m_Size.y += m_Position.y - mousePos.y;
				m_Position.y = mousePos.y;
				if (m_Position.y < Input::GetWindowSize().y/2)
				{
					m_Position.y = Input::GetWindowSize().y / 2;
					m_Size.y = Input::GetWindowSize().y / 2;
				}
				break;
			}		
			updateBuffers();
		}
	}

	bool Panel::OnLeftClick(const glm::vec2& mousePos)
	{
		for (auto& widg : m_Widget)
		{
			if (widg.second->OnLeftClick(mousePos))
			{
				return true;
			}
		}
		glm::vec4 coords = getCoords();
		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			updateBuffers();
			m_Clicked = true;
			return true;
		}
		return false;
	}

	bool Panel::OnRightClick(const glm::vec2& mousePos)
	{
		for (auto& widg : m_Widget)
		{
			if (widg.second->OnRightClick(mousePos))
			{
				return true;
			}
		}
		glm::vec4 coords = getCoords();
		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			return false;
		}
		return false;
	}

	bool Panel::OnRelease()
	{
		if (m_Clicked)
		{
			handleResize();
			return true;
		}
		for (auto& widg : m_Widget)
		{	
			if (widg.second->Release())
				return true;
		}
		return false;
	}

	void Panel::Set(const glm::vec2& pos, const glm::vec2& size)
	{
		m_Position = pos;
		m_Size = size;
		updateBuffers();
	}
	
	Ref<Panel> Panel::Create(const glm::vec2& position, const glm::vec2& size, const glm::vec3& labelColor, const std::string& label, ElementPosition pos)
	{
		Ref<Panel> window = std::make_shared<Panel>(position, size, labelColor, label,pos);
		return window;
	}

	void Panel::updateBuffers()
	{
		
		m_PanelQuad.SetPosition(m_Position, glm::vec2(m_Size.x, Font::GetFont("Arial")->GetData().lineHeight * 0.2f + 5));
		m_Quad.SetPosition(m_Position, m_Size);
		m_Label.SetPosition(glm::vec2(m_Position.x, m_Position.y + 4));

		GuiApplication::Get()->UpdateVertexBuffer(m_PanelIndex, &m_PanelQuad);
		GuiApplication::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
		GuiApplication::Get()->UpdateLabelBuffer(m_Index, m_Label.GetVertice(0), m_Label.GetNumChar());
	}

	void Panel::handleResize()
	{		
		xOffset = 10;
		yOffset = 2 * m_Label.GetSize().y;

		if (m_Size.x < m_Label.GetSize().x)
		{
			switch (m_Pos)
			{
			case ElementPosition::LEFT:
				m_Size.x = m_Label.GetSize().x;
				break;
			case ElementPosition::RIGHT:
				m_Position.x -= m_Label.GetSize().x - m_Size.x;
				m_Size.x += m_Label.GetSize().x - m_Size.x;
				break;
			}
			updateBuffers();
		}
		else if (m_Size.y < m_Label.GetSize().y)
		{
			switch (m_Pos)
			{
			case ElementPosition::BOTTOM:
				m_Position.y = Input::GetWindowSize().y - m_Label.GetSize().y;
				m_Size.y = m_Label.GetSize().y;
			}
			updateBuffers();
		}
		for (auto& widget : m_Widget)
		{
			if (widget.second->GetSize().y > rowSize)
				rowSize = widget.second->GetSize().y;

			if (m_Position.x + xOffset + widget.second->GetSize().x > m_Position.x + m_Size.x)
			{
				xOffset = 10;
				yOffset += rowSize;
				rowCount = 0;
			}
			if (   m_Position.x + xOffset + widget.second->GetSize().x > m_Position.x + m_Size.x 
				|| m_Position.y + yOffset + widget.second->GetSize().y > m_Position.y + m_Size.y)
			{
				widget.second->StopRender();
			}
			else
			{
				widget.second->SetPosition(glm::vec2(m_Position.x + xOffset, m_Position.y + yOffset));
			}
			xOffset += widget.second->GetSize().x + 10;
			rowCount++;
		}
		m_Clicked = false;
		
	}

	glm::vec4 Panel::getCoords()
	{
		switch (m_Pos)
		{
		case ElementPosition::LEFT:
			return glm::vec4(m_Position.x + m_Size.x - m_PanelSize.y,
				m_Position.y + m_Size.y,
				m_Position.x + m_Size.x,
				m_Position.y);
			break;
		case ElementPosition::RIGHT:
			return glm::vec4(m_Position.x,
				m_Position.y + m_Size.y,
				m_Position.x + m_PanelSize.y,
				m_Position.y);
			break;
		case ElementPosition::BOTTOM:
			return glm::vec4(m_Position.x,
				m_Position.y + m_PanelSize.y,
				m_Position.x + m_Size.x,
				m_Position.y);
			break;
		}		
	}

}