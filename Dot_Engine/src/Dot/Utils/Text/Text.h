#pragma once
#include "Font.h"
#include "Dot/Renderer/Renderable/Renderable2D.h"

namespace Dot {
	class Text
	{
	public:
		Text(const std::string& font, std::string text, const glm::vec2 position, const glm::vec2 size,const glm::vec3& color,int maxCharacter);

		void SetPosition(const glm::vec2& pos);
		void Move(const glm::vec2& pos);
		void Push(std::string text, const glm::vec3& color);
		void PushChar(const char character, const glm::vec3& color);
		void Pop();
		void Clear();
		void SetPositionInBuffer(int offset);
		void RestartCurserX() { m_CurserX = m_Position.x; }
		void RestartCurserY() { m_CurserY = 0; }

		const std::string& GetText() const { return m_Text; }
		const glm::vec2& GetSize() const { return m_SizeText; }
		const glm::vec2& GetPosition() const { return m_Position; }
		const std::string& GetFont() const { return m_Font; }
		const int GetNumChar() const { return m_Text.size(); };
		const QuadVertex2D* GetVertice(int index) { return &m_Vertice[index]; }
	private:
		std::string m_Font;
		glm::vec2 m_Position;
		glm::vec2 m_SizeText;
		glm::vec2 m_Size;

		unsigned int m_MaxChar = 0;
		unsigned int m_PositionInBuffer = 0;
		float m_LineHeight;

		double m_CurserX;
		double m_CurserY;

	private:
		std::vector <QuadVertex2D> m_Vertice;
		std::string m_Text;

	};

}