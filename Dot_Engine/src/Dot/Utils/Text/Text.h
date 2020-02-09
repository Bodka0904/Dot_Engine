#pragma once
#include "Font.h"
#include "Dot/Renderer/Renderable/Renderable2D.h"

namespace Dot {
	class Text
	{
	public:
		Text(const std::string& font, std::string text, const glm::vec2 position, const glm::vec2 size, const glm::vec3& color, unsigned int maxCharacter,unsigned int numCharRow = 50);

		void Push(std::string text, const glm::vec3& color);
		void PushCharacter(const char character, const glm::vec3& color);
		void Pop();
		void Clear();
	
		void RestartCurserX();
		void RestartCurserY();
		void SetPosition(const glm::vec2& pos);
		void Move(const glm::vec2& pos);
		void SetNumCharPerRow(unsigned int numChar) { m_NumCharPerRow = numChar; }

		const std::string& GetText() const { return m_Text; }
		const glm::vec2& GetSize() const { return m_Size; }
		const glm::vec2& GetCharSize() const { return m_CharSize; }
		const glm::vec2& GetPosition() const { return m_Position; }
		const std::string& GetFont() const { return m_Font; }
		const int GetNumChar() const { return int(m_Text.size()); };
		const QuadVertex2D* GetVertice(int index) { return &m_Vertice[index]; }
	private:
		std::string m_Text;
		std::string m_Font;
		glm::vec2 m_Position;
		glm::vec2 m_CharSize;
		glm::vec2 m_Size;

		float m_LineHeight;
		unsigned int m_MaxChar;
		unsigned int m_NumCharPerRow;
		unsigned int m_CurrentCharInRow;
		unsigned int m_PositionInBuffer;

		float m_CurserX;
		float m_CurserY;
	private:
		std::vector <QuadVertex2D> m_Vertice;
	};

}