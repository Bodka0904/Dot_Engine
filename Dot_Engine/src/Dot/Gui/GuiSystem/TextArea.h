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

		TextArea(const std::string& label,const glm::vec2& position, const glm::vec2& size, TYPE type, float labelsize);
		virtual bool MouseHoover(const glm::vec2& mousePos) override;
	
		virtual void ClickHandle() override;
		virtual void Exit()override;
		virtual void SetPosition(const glm::vec2& pos) override;
		virtual void Move(const glm::vec2 pos) override;
		void SetText(const std::string& text);

		virtual const glm::vec2& GetPosition() override { return m_Position; }
		virtual const glm::vec2& GetSize() override { return m_Size; }
		virtual const glm::vec2& GetLabelSize() override;
		virtual const unsigned int GetIndex() override { return m_Index; }

		void TakeInput(KeyPressedEvent& event);
		const bool& GetClicked() const;
		
		template<typename T>
		const T GetValue();

		template <>
		const float GetValue<float>()
		{
			return convertToFloat();
		}
		template <>
		const int GetValue<int>()
		{
			return convertToInt();
		}
		template <>
		const std::string GetValue<std::string>()
		{
			return m_Text->GetText();
		}	

		static TextArea& Get(const std::string& label);
		static TextArea& GetWrapped(const std::string& wrapper, const std::string& label);
		static void Create(const std::string& label, const glm::vec2& position, const glm::vec2& size, TYPE type,float labelsize = 0.18f);
	private:
		glm::vec4 getCoords();
		float convertToFloat();
		int	  convertToInt();
	private:
		TYPE m_Type;
		Ref<Text> m_Label;
		Ref<Text> m_Text;
		QuadVertex m_Quad;

		glm::vec2 m_Position;
		glm::vec2 m_Size;

		bool m_Clicked = false;
		int m_TexOffset = 0;
		unsigned int m_Index = 0;
	};
}