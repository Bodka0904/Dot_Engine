#pragma once
#include <glm/glm.hpp>

#include "Dot/Utils/Text/Text.h"
#include "Gui.h"

namespace Dot {

	class Checkbox : public Widget
	{
	public:
		Checkbox(const glm::vec2& position, const glm::vec2& size,float labelsize = 0.2f);
		virtual bool MouseHoover(const glm::vec2& mousePos) override;	
		virtual void UpdateData() override;
		virtual void ClickHandle() override;
		virtual void Exit()override;
		virtual void SetLabel(const Ref<Text> label) override;
		virtual void Move(const glm::vec2 pos) override { m_Position += pos; };
		virtual void SetIndex(const unsigned int index) override { m_Index = index; };
		virtual void SetPosition(const glm::vec2& pos) override { m_Position = pos; };

		virtual const glm::vec2& GetPosition() override { return m_Position; }
		virtual const glm::vec2& GetSize() override { return m_Size; }
		virtual const glm::vec2& GetLabelSize() override;

		virtual const unsigned int GetIndex() override { return m_Index; }
		const bool& GetClicked() const { return m_Clicked; }

		static Checkbox& Get(const std::string& label);
		static Checkbox& GetWrapped(const std::string& wrapper, const std::string& label);
		static void Create(const std::string& label, const glm::vec2& position, const glm::vec2& size,float labelsize = 0.2f);
	private:
		glm::vec4 getCoords();
	private:
		Ref<Text> m_Label;

		glm::vec2 m_Position;
		glm::vec2 m_Size;

		bool m_Clicked = false;
		int m_TexOffset = 0;
		unsigned int m_Index = 0;

	};
}