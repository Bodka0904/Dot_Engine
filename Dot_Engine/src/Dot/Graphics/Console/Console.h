#pragma once
#include "Dot/Graphics/Text/DynamicText.h"
#include "Dot/Graphics/Renderer/Renderable2D.h"

#include <glm/glm.hpp>

namespace Dot {
	class Console : public Renderable2D
	{
	public:
		Console(const glm::vec2& position,const glm::vec2& size,int maxChar, const std::string& font,const glm::vec2& fontsize);

		void PrintInfo(const std::string text);
		void PrintWarning(const std::string text);
		void PrintError(const std::string text);

		bool Update();
		bool HandleLeftClick();
		bool HandleRighClick();

		void HandleRelease();
		void SetPosition(const glm::vec2& pos);

		virtual const QuadVertex& GetQuad() override { return m_Quad; }
	private:
		bool MouseHoover(const glm::vec2& mousePos);
		bool MouseResize(const glm::vec2& mousePos);
		glm::vec4 GetCoords();
	private:
		DynamicText m_Text;

		glm::vec2 m_Position;
		glm::vec2 m_Size;

		int m_LineLen = 0;
		int m_NumLines = 0;
		int m_MaxChar = 0;

		bool m_Resize = false;
		bool m_Move = false;

		glm::vec2 m_MousePosition;
	private:
		QuadVertex m_Quad;
	};
}