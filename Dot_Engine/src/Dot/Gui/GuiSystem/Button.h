#pragma once
#include <glm/glm.hpp>

#include "Dot/Utils/Text/Text.h"
#include "Gui.h"

namespace Dot {
	class Button : public Widget
	{
	public:
		Button(const std::string& label,const glm::vec2& position, const glm::vec2& size,float labelsize);
		~Button();
		virtual bool MouseHoover(const glm::vec2& mousePos) override;
		virtual void ClickHandle() override { m_Clicked = true; }
		virtual void Minimize()override;
		virtual void Move(const glm::vec2 pos) override;
		virtual void SetPosition(const glm::vec2& pos)override;

		virtual const glm::vec2& GetPosition() override { return m_Position; }
		virtual const glm::vec2& GetSize() override { return m_Size; }
		virtual const glm::vec2& GetLabelSize() override;

		const bool GetClicked();
		virtual const unsigned int GetIndex() override { return m_Index; }

		static Button& Get(const std::string& label);
		static Button& GetWrapped(const std::string& wrapper, const std::string& label);
		static void Create(const std::string& label, const glm::vec2& position, const glm::vec2& size,float labelsize = 0.18f);
	private:
		glm::vec4 getCoords();
	private:
		Ref<Text> m_Label;
		QuadVertex m_Quad;

		glm::vec2 m_Position;
		glm::vec2 m_Size;
		
		bool m_Clicked = false;
		unsigned int m_Index = 0;
	};
}

