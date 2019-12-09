#pragma once
#include <glm/glm.hpp>

#include "Dot/Graphics/Text/Text.h"
#include "Gui.h"


namespace Dot {
	
	class Arrbutton : public Widget
	{
	public:
		Arrbutton(const std::string& label, const glm::vec2& position, const glm::vec2& size,float labelsize = 0.2f);
		virtual bool MouseHoover(const glm::vec2& mousePos) override;
		virtual void Move(const glm::vec2 pos) override;
		virtual void SetPosition(const glm::vec2& pos) override;
		virtual void ClickHandle() override { *m_Clicked = true;}
		virtual void SetIndex(const unsigned int index) override { m_Index = index; };
		virtual const glm::vec2& GetPosition() override { return m_Position; }
		virtual const glm::vec2& GetSize() override { return m_Size; }
		virtual const glm::vec2& GetLabelSize() override;

		const bool LeftClicked();
		const bool RightClicked();

		const int GetIndex() const { return m_Index; }

		static Arrbutton& Get(const std::string& label);
		static Arrbutton& GetWrapped(const std::string& wrapper, const std::string& label);
		static void Create(const std::string& label, const glm::vec2& position, const glm::vec2& size);
	private:
		glm::vec4 GetCoords();
	private:
		Text m_Label;

		glm::vec2 m_Position;
		glm::vec2 m_Size;

		bool * m_Clicked = false;
		bool m_LeftClick = false;
		bool m_RightClick = false;
		unsigned int m_Index = 0;

	};
}