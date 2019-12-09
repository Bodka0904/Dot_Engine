#include "stdafx.h"
#include "Console.h"
#include "Dot/Input.h"

#include <math.h>

namespace Dot {
	Console::Console(const glm::vec2& position, const glm::vec2& size,int maxChar,const std::string& font, const glm::vec2& fontsize)
		:
		m_Position(position),
		m_Size(size),
		m_MaxChar(maxChar),
		m_Text(font,"Console:",position,fontsize,maxChar),
		m_Quad(position,size,NULL)
	{
		m_MousePosition = m_Position;
		m_NumLines = floor(size.y / (Font::GetFont(font)->GetData().lineHeight * fontsize.y));
	}
	void Console::PrintInfo(const std::string text)
	{
	}
	void Console::PrintWarning(const std::string text)
	{
	}
	void Console::PrintError(const std::string text)
	{
	}
	bool Console::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = GetCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			m_MousePosition = mousePos;
			return true;
		}
		return false;
	}
	bool Console::Update()
	{
		glm::vec2 mousePos;
		mousePos.x = Input::GetMouseX();
		mousePos.y = Input::GetMouseY();
		
		if (m_Resize)
		{
			m_Size = abs(mousePos - m_Position);
			m_Quad.m_Vertices[0].position = m_Position;
			m_Quad.m_Vertices[1].position = glm::vec2(m_Position.x + m_Size.x, m_Position.y);
			m_Quad.m_Vertices[2].position = glm::vec2(m_Position + m_Size);
			m_Quad.m_Vertices[3].position = glm::vec2(m_Position.x, m_Position.y + m_Size.y);

			m_NumLines = floor(m_Size.y / (Font::GetFont(m_Text.GetFont())->GetData().lineHeight * m_Text.GetSize().y));
			
			return m_Resize;
		}
		else if (m_Move)
		{
			glm::vec2 dif = mousePos - m_MousePosition;
			m_Position += dif ;
			m_MousePosition = mousePos;

			m_Quad.m_Vertices[0].position = m_Position;
			m_Quad.m_Vertices[1].position = glm::vec2(m_Position.x + m_Size.x, m_Position.y);
			m_Quad.m_Vertices[2].position = glm::vec2(m_Position + m_Size);
			m_Quad.m_Vertices[3].position = glm::vec2(m_Position.x, m_Position.y + m_Size.y);
	
			m_NumLines = floor(m_Size.y / (Font::GetFont(m_Text.GetFont())->GetData().lineHeight * m_Text.GetSize().y));

			return m_Move;
		}
		return false;
	}
	
	bool Console::HandleLeftClick()
	{
		return MouseResize(glm::vec2(Input::GetMouseX(), Input::GetMouseY()));
	}
	bool Console::HandleRighClick()
	{
		if (MouseHoover(glm::vec2(Input::GetMouseX(), Input::GetMouseY())))
		{

			m_Move = true;
			return true;
		}
		return false;
	}
	bool Console::MouseResize(const glm::vec2& mousePos)
	{
		int offset = 20;

		if (mousePos.x < m_Position.x + m_Size.x && mousePos.x > m_Position.x + m_Size.x - offset
			&& mousePos.y < m_Position.y + m_Size.y && mousePos.y > m_Position.y + m_Size.y - offset)
		{	
			m_Resize = true;
			return true;
		}
		return false;
	}
	

	void Console::HandleRelease()
	{
		m_Resize = false;
		m_Move = false;
	}
	
	void Console::SetPosition(const glm::vec2& pos)
	{
	}
	glm::vec4 Console::GetCoords()
	{
		return glm::vec4(m_Position.x,
			m_Position.y + m_Size.y,
			m_Position.x + m_Size.x,
			m_Position.y);
	}
}