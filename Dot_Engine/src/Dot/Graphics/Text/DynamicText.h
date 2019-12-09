#pragma once
#include "Dot/Graphics/Renderer/Buffers/ArrayBuffer.h"
#include "Font.h"

namespace Dot {
	class DynamicText
	{
	public:
		DynamicText(const std::string& font, std::string text, const glm::vec2 position, const glm::vec2 size, int MaxCharacter);
		
		void Update(std::string text);
		void SetPositionInBuffer(int offset);
		void RestartCurserX() { m_CurserX = m_Position.x; }
		void RestartCurserY() { m_CurserY = 0; }

		const glm::vec2& GetSize() const { return m_Size; }
		const std::string& GetFont() const { return m_Font; }
		const int GetNumChar() const { return m_NumChars; };
		const Ref<ArrayBuffer>& GetVAO() const { return m_VAO; }
	private:
		std::string m_Font;
		glm::vec2 m_Position;
		glm::vec2 m_Size;
	
		int m_MaxChar = 0;
		unsigned int m_PositionInBuffer = 0;
		unsigned int m_NumChars = 0;

		double m_CurserX;
		double m_CurserY;

	private:
		Ref<ArrayBuffer> m_VAO;

	private:
		std::vector <glm::vec2> m_Vertice;
		std::vector <glm::vec2> m_TexCoord;
	
	};

}