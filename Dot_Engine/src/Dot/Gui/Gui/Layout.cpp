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
					panel->Set(m_Layout[m_ResizeIndex + 1].position.x, m_Layout[m_ResizeIndex + 1].size.x);
				for (auto& win : m_Layout[m_ResizeIndex + 1].m_Window)
					win->Set(m_Layout[m_ResizeIndex + 1].position.x, m_Layout[m_ResizeIndex + 1].size.x);
				for (auto& console : m_Layout[m_ResizeIndex + 1].m_Console)
					console->Set(m_Layout[m_ResizeIndex + 1].position.x, m_Layout[m_ResizeIndex + 1].size.x);
			}
			for (auto& panel : m_Layout[m_ResizeIndex].m_Panel)
				panel->Set(m_Layout[m_ResizeIndex].position.x, m_Layout[m_ResizeIndex].size.x);
			for (auto& win : m_Layout[m_ResizeIndex].m_Window)
				win->Set(m_Layout[m_ResizeIndex].position.x, m_Layout[m_ResizeIndex].size.x);
			for (auto& console : m_Layout[m_ResizeIndex].m_Console)
				console->Set(m_Layout[m_ResizeIndex].position.x, m_Layout[m_ResizeIndex].size.x);
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
		: position(Position), size(Size), elements(Elements)
	{
		glm::vec2 winSize = Input::GetWindowSize();
		size *= winSize;
		position *= winSize;
		for (auto& el : elements)
		{
			el.height *= winSize.y;
		}
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
	glm::vec4 SubLayout::getCoords()
	{
		return glm::vec4(position.x,
			position.y + size.y,
			position.x + size.x,
			position.y);
	}
}


