#include "stdafx.h"
#include "GuiBlock.h"

#include "Widgets/Panel.h"
#include "Dot/Core/Input.h"


namespace Dot {
	GuiBlock::GuiBlock()	
	{
		glm::vec2 winSize = Input::GetWindowSize();
		m_LayoutBlock[LayoutBlock::LEFT].Size = glm::vec2(0.2 * winSize.x, winSize.y);
		m_LayoutBlock[LayoutBlock::LEFT].Position = glm::vec2(0, 0);
		
		m_LayoutBlock[LayoutBlock::MIDDLE].Size = glm::vec2(0.6 * winSize.x, winSize.y);
		m_LayoutBlock[LayoutBlock::MIDDLE].Position = glm::vec2(0.2*winSize.x, 0);

		m_LayoutBlock[LayoutBlock::RIGHT].Size = glm::vec2(0.2 * winSize.x, winSize.y);
		m_LayoutBlock[LayoutBlock::RIGHT].Position = glm::vec2(0.8*winSize.x, winSize.y);
	}
	GuiBlock::~GuiBlock()
	{
	}

	bool GuiBlock::OnLeftClick(const glm::vec2& mousePos)
	{
		glm::vec4 coordsLeft = m_LayoutBlock[LayoutBlock::LEFT].getCoords();
		if (mousePos.x >= coordsLeft.x &&
			(mousePos.x <= coordsLeft.z - 10
				|| mousePos.x <= coordsLeft.z + 10)
			&& mousePos.y <= coordsLeft.y && mousePos.y >= coordsLeft.w)
		{
			std::cout << "RESIZE LEFT" << std::endl;
			m_ResizeLeft = true;
			return true;
		}
		m_ResizeLeft = false;

		glm::vec4 coordsRight = m_LayoutBlock[LayoutBlock::RIGHT].getCoords();
		if (mousePos.x >= coordsRight.x &&
			(mousePos.x <= coordsRight.z - 10
				|| mousePos.x <= coordsRight.z + 10)
			&& mousePos.y <= coordsRight.y && mousePos.y >= coordsRight.w)
		{
			m_ResizeRight = true;
			return true;
		}
		m_ResizeRight = false;
		return false;
		
		
		
		for (auto& it : m_Console)
		{
			if (it.second->OnLeftClick(mousePos))
			{
				break;
			}
		}

		for (auto& it : m_Panel)
		{
			if (it.second->OnLeftClick(mousePos))
			{
				break;
			}
		}
		for (auto& it : m_Widget)
		{
			if (it.second->OnLeftClick(mousePos))
			{					
				break;
			}
		}
	}

	void GuiBlock::OnRightClick()
	{
		glm::vec2 mousePos(Input::GetMouseX(), Input::GetMouseY());
		for (auto& it : m_Panel)
		{
			if (it.second->OnRightClick(mousePos))
			{
				break;
			}
		}
		for (auto& it : m_Widget)
		{
			if (it.second->OnRightClick(mousePos))
			{
				break;
			}
		}
	}

	void GuiBlock::OnLeftRelease()
	{
		m_ResizeLeft = false;
		m_ResizeRight = false;
		glm::vec2 mousePos(Input::GetMouseX(), Input::GetMouseY());
		for (auto& it : m_Console)
		{
			if (it.second->OnRelease())
			{
				break;
			}
		}
		for (auto& it : m_Panel)
		{
			if (it.second->OnRelease())
			{
				break;
			}
		}
		for (auto& it : m_Widget)
		{
			if (it.second->Release())
			{
				break;
			}
		}
	}

	void GuiBlock::SetLayout(GuiLayout& layout)
	{		
		m_Layout = layout;
		glm::vec2 winSize = Input::GetWindowSize();
		for (auto& element : layout.Elements)
		{
			if (element.TYPE == ElementType::PANEL)
			{
				m_Panel[element.Name] = Panel::Create(element.Position, element.Size, glm::vec3(1, 1, 1), element.Name, element.POS);
				m_LayoutBlock[LayoutBlock::LEFT].Panel[element.Name] = m_Panel[element.Name];
			}
			else if (element.TYPE == ElementType::CONSOLE)
				m_Console[element.Name] = Console::Create(element.Position, element.Size, glm::vec3(1, 1, 1), element.Name, element.POS);
		}	
	}
	void GuiBlock::HandleResize(const glm::vec2& mousePos)
	{
		if (m_ResizeLeft)
		{
			m_LayoutBlock[LayoutBlock::LEFT].Size.x = abs(mousePos.x - m_LayoutBlock[LayoutBlock::LEFT].Position.x);
			m_LayoutBlock[LayoutBlock::MIDDLE].Size.x = m_LayoutBlock[LayoutBlock::MIDDLE].Position.x - mousePos.x;
			m_LayoutBlock[LayoutBlock::MIDDLE].Position.x = mousePos.x;
			
			for (auto& panel : m_LayoutBlock[LayoutBlock::LEFT].Panel)
				panel.second->Set(m_LayoutBlock[LayoutBlock::LEFT].Position, m_LayoutBlock[LayoutBlock::LEFT].Size);
			for (auto& panel : m_LayoutBlock[LayoutBlock::MIDDLE].Panel)
				panel.second->Set(m_LayoutBlock[LayoutBlock::MIDDLE].Position, m_LayoutBlock[LayoutBlock::MIDDLE].Size);

		}
		else if (m_ResizeRight)
		{
			std::cout << "RESIZE RIGHT" << std::endl;
			m_LayoutBlock[LayoutBlock::MIDDLE].Size.x = abs(mousePos.x - m_LayoutBlock[LayoutBlock::MIDDLE].Position.x);
			m_LayoutBlock[LayoutBlock::RIGHT].Size.x = m_LayoutBlock[LayoutBlock::RIGHT].Position.x - mousePos.x;
			m_LayoutBlock[LayoutBlock::RIGHT].Position.x = mousePos.x;
		}
	}
	void GuiBlock::AddWidget(const std::string& name, const Ref<Widget> widget)
	{
		D_ASSERT(m_Widget.find(name) == m_Widget.end(), "Widget with name %s already exists", name.c_str());
		m_Widget[name] = widget;
	}

	void GuiBlock::AddPanel(const std::string& name, Ref<Panel> panel)
	{
		D_ASSERT(m_Panel.find(name) == m_Panel.end(), "Panel with name %s already exists", name.c_str());
		m_Panel[name] = panel;
	}

	void GuiBlock::AddConsole(const std::string& name, Ref<Console> console)
	{
		D_ASSERT(m_Console.find(name) == m_Console.end(), "Console with name %s already exists", name.c_str());
		m_Console[name] = console;
	}

	void GuiBlock::AddWindow(const std::string& name, Ref<GuiWindow> window)
	{
		D_ASSERT(m_Window.find(name) == m_Window.end(), "Window with name %s already exists", name.c_str());
		m_Window[name] = window;
	}

	bool GuiBlock::MouseHoover(glm::vec2& mousePos)
	{ 
		glm::vec4 coords = getCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			return true;
		}
		return false;
	}

	glm::vec4 GuiBlock::getCoords()
	{
		return glm::vec4();
	}

	GuiBlock::LayoutBlock::LayoutBlock(const glm::vec2& position, const glm::vec2& size)
		: Size(size),Position(position)
	{
	}


	glm::vec4 GuiBlock::LayoutBlock::getCoords()
	{
		return glm::vec4(Position.x,
			Position.y + Size.y,
			Position.x + Size.x,
			Position.y);
	}

}