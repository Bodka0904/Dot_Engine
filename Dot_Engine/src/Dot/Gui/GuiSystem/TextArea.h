#pragma once
#include <glm/glm.hpp>

#include "Dot/Utils/Text/Text.h"
#include "Dot/Core/Events/Event.h"
#include "Gui.h"

namespace Dot {

	class TextArea : public Widget
	{
	public:
		enum class TYPE
		{
			INT,
			FLOAT,
			TEXT,
		};

		TextArea(const glm::vec2& position, const glm::vec2& size, TYPE type, float labelsize = 0.2f);
		virtual bool MouseHoover(const glm::vec2& mousePos) override;
		virtual void UpdateData() override;
	
		virtual void ClickHandle() override;
		virtual void Exit()override;
		virtual void SetIndex(const unsigned int index) override { m_Index = index; };
		virtual void SetPosition(const glm::vec2& pos) override { m_Position = pos; };
		virtual void Move(const glm::vec2 pos) override { m_Position += pos; };
		virtual void SetLabel(const Ref<Text> label)override;
		void SetTextHandle(const Ref<Text> text);
		void SetText(const std::string& text);

		virtual const glm::vec2& GetPosition() override { return m_Position; }
		virtual const glm::vec2& GetSize() override { return m_Size; }
		virtual const glm::vec2& GetLabelSize() override;
		virtual const unsigned int GetIndex() override { return m_Index; }

		const void TakeInput(KeyPressedEvent& event);
		const bool& GetClicked() const;
		
		const std::string GetText()
		{
			std::stringstream ss(m_Text->GetText());
			std::string val;
			ss >> val;
			return val;
		}
		
		template<typename T>
		const T GetValue()
		{
			switch (m_Type)
			{
			case Dot::TextArea::TYPE::INT:
				return (T)convertToInt();
			case Dot::TextArea::TYPE::FLOAT:
				return (T)convertToFloat();
			}
		}

		static TextArea& Get(const std::string& label);
		static TextArea& GetWrapped(const std::string& wrapper, const std::string& label);
		static void Create(const std::string& label, const glm::vec2& position, const glm::vec2& size, TYPE type,float labelsize = 0.2f);
	private:
		glm::vec4 getCoords();
		float convertToFloat();
		int	  convertToInt();
	private:
		Ref<Text> m_Label;
		Ref<Text> m_Text;

		glm::vec2 m_Position;
		glm::vec2 m_Size;

		bool m_Clicked = false;
		int m_TexOffset = 0;
		unsigned int m_Index = 0;
		TYPE m_Type;

	};
}