#include "stdafx.h"
#include "Layout.h"

#include "Dot/Core/Input.h"

namespace Dot {
	Layout::Layout(const std::initializer_list<SubLayout>& layout)
		: m_Layout(layout)
	{
		std::sort(m_Layout.begin(), m_Layout.end(),m_Cmp);
	}
	void Layout::HandleResize(const glm::vec2& mousePos)
	{
		if (m_ResizeIndex != -1)
		{
			m_Layout[m_ResizeIndex].size.x = abs(mousePos.x - m_Layout[m_ResizeIndex].position.x);
			if (m_ResizeIndex < m_Layout.size() - 1)
			{
				m_Layout[m_ResizeIndex+1].size.x = m_Layout[m_ResizeIndex + 1].position.x + m_Layout[m_ResizeIndex + 1].size.x - mousePos.x;
				m_Layout[m_ResizeIndex+1].position.x = mousePos.x;

				for (auto& panel : m_Layout[m_ResizeIndex+1].m_Panel)
					panel->SetX(m_Layout[m_ResizeIndex + 1].position.x, m_Layout[m_ResizeIndex + 1].size.x);
				for (auto& win : m_Layout[m_ResizeIndex + 1].m_Window)
					win->SetX(m_Layout[m_ResizeIndex + 1].position.x, m_Layout[m_ResizeIndex + 1].size.x);
				for (auto& console : m_Layout[m_ResizeIndex + 1].m_Console)
					console->SetX(m_Layout[m_ResizeIndex + 1].position.x, m_Layout[m_ResizeIndex + 1].size.x);
			}
			for (auto& panel : m_Layout[m_ResizeIndex].m_Panel)
				panel->SetX(m_Layout[m_ResizeIndex].position.x, m_Layout[m_ResizeIndex].size.x);
			for (auto& win : m_Layout[m_ResizeIndex].m_Window)
				win->SetX(m_Layout[m_ResizeIndex].position.x, m_Layout[m_ResizeIndex].size.x);
			for (auto& console : m_Layout[m_ResizeIndex].m_Console)
				console->SetX(m_Layout[m_ResizeIndex].position.x, m_Layout[m_ResizeIndex].size.x);
		}
	}
	void Layout::OnLeftClick(const glm::vec2& mousePos)
	{
		for (int i = 0; i < m_Layout.size(); ++i)
		{
			if (m_Layout[i].MouseHoover(mousePos))
			{
				m_ResizeIndex = i;
				break;
			}
		}
	}
	void Layout::OnLeftRelease()
	{
		m_ResizeIndex = -1;
	}
	SubLayout::SubLayout(const glm::vec2& Position, const glm::vec2& Size, const std::initializer_list<Element>& Elements)
		: position(Position), size(Size), elements(Elements),defaultPosition(Position),defaultSize(size)
	{
		glm::vec2 winSize = glm::vec2(float(Input::GetWindowSize().first), float(Input::GetWindowSize().second));
		size *= winSize;
		position *= winSize;


	}
	bool SubLayout::MouseHoover(const glm::vec2 mousePos)
	{
		glm::vec4 coords = getCoords();
		if (mousePos.x <= coords.z + 10 && mousePos.x >= coords.z - 10)
		{
			return true;
		}
		return false;
	}
	void SubLayout::Restart()
	{
		glm::vec2 winSize = glm::vec2(float(Input::GetWindowSize().first), float(Input::GetWindowSize().second));
		position = defaultPosition * winSize;
		size = defaultSize * winSize;		

		glm::vec2 newPos = position;

		int panelCounter = 0;
		int windowCounter = 0;
		int consoleCounter = 0;
		for (int i = 0; i < elements.size(); ++i)
		{
			float height = elements[i].height * Input::GetWindowSize().second;
			if (elements[i].type == ElementType::PANEL)
			{
				m_Panel[panelCounter]->SetX(newPos.x, size.x);
				m_Panel[panelCounter]->SetY(newPos.y,height);
				panelCounter++;
			}
			else if (elements[i].type == ElementType::WINDOW)
			{
				m_Window[windowCounter]->SetX(newPos.x, size.x);
				m_Window[windowCounter]->SetY(newPos.y, height);
				windowCounter++;
			}
			else
			{
				m_Console[consoleCounter]->SetX(newPos.x, size.x);
				m_Console[consoleCounter]->SetY(newPos.y, height);
				consoleCounter++;
			}
			newPos.y += height;
		}
	}
	glm::vec4 SubLayout::getCoords()
	{
		return glm::vec4(position.x,
			position.y + size.y,
			position.x + size.x,
			position.y);
	}
}


