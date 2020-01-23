#include "stdafx.h"
#include "GuiBlock.h"

#include "Widgets/Panel.h"
#include "Dot/Core/Input.h"


namespace Dot {
	GuiBlock::GuiBlock()
	{
	}
	GuiBlock::~GuiBlock()
	{
	}

	bool GuiBlock::OnLeftClick(const glm::vec2& mousePos)
	{
		m_Layout.OnLeftClick(mousePos);

		for (auto& it : m_Panel)
		{
			if (it.second->OnLeftClick(mousePos))
				return true;			
		}
		for (auto& it : m_Console)
		{
			if (it.second->OnLeftClick(mousePos))
				return true;
			
		}
		for (auto& it : m_Widget)
		{
			if (it.second->OnLeftClick(mousePos))
				return true;		
		}

		return false;
	}

	void GuiBlock::OnLeftRelease()
	{
		m_Layout.OnLeftRelease();
		for (auto& it : m_Widget)
		{
			if (it.second->Release())
				break;
			
		}
	}

	void GuiBlock::SetLayout(Layout& layout)
	{
		m_Layout = layout;
		for (auto& lay : m_Layout.m_Layout)
		{
			glm::vec2 newPos = lay.position;
			for (auto& el : lay.elements)
			{
				if (el.type == ElementType::PANEL)
				{
					m_Panel[el.name] = Panel::Create(newPos, glm::vec2(lay.size.x, el.height), glm::vec3(1, 1, 1), el.name);
					lay.m_Panel.push_back(m_Panel[el.name].get());
				}
				else if (el.type == ElementType::WINDOW)
				{
					m_Window[el.name] = GuiWindow::Create(newPos, glm::vec2(lay.size.x, el.height), glm::vec3(1, 1, 1), el.name);
					lay.m_Window.push_back(m_Window[el.name].get());
				}
				else
				{
					m_Console[el.name] = Console::Create(newPos, glm::vec2(lay.size.x, el.height), glm::vec3(1, 1, 1), el.name);
					lay.m_Console.push_back(m_Console[el.name].get());
				}
				newPos.y += el.height;
			}
		}
	}
	void GuiBlock::HandleResize(const glm::vec2& mousePos)
	{
		m_Layout.HandleResize(mousePos);
	}
	void GuiBlock::AddWidget(const std::string& name, const Ref<Widget> widget)
	{
		D_ASSERT(m_Widget.find(name) == m_Widget.end(), "Widget with name %s already exists", name.c_str());
		m_Widget[name] = widget;
	}


}