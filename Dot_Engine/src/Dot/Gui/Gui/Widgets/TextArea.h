#pragma once
#include "Widget.h"
#include "Dot/Renderer/Renderable/Renderable2D.h"
#include "Dot/Utils/Text/Text.h"
#include "Dot/Core/Events/Event.h"

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

		TextArea(const std::string& label, const glm::vec2& position, const glm::vec2& size, const glm::vec3& color,const glm::vec3& textColor);
		~TextArea();
		virtual bool OnLeftClick(const glm::vec2& mousePos) override;
		virtual bool MouseHoover(const glm::vec2& mousePos);
		virtual bool Release() override;
		virtual void Move(const glm::vec2& pos);
		virtual void SetPosition(const glm::vec2& pos);
		virtual void SetColor(const glm::vec3& color);
		virtual void SetSize(const glm::vec2& size);
		virtual void StopRender() override;
		void TakeInput(KeyPressedEvent& event,TYPE type);
		virtual const glm::vec2& GetSize();
		
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
			return m_Text.GetText();
		}

		static Ref<Widget> Create(const std::string& label, const glm::vec2& position, const glm::vec2& size, const glm::vec3& color,const glm::vec3& textColor);
	private:
		void updateBuffers();
		glm::vec4 getCoords();
		float convertToFloat();
		int	  convertToInt();
	private:
		QuadVertex2D m_Quad;
		Text m_Label;
		Text m_Text;

		glm::vec2 m_Position;
		glm::vec2 m_Size;
		glm::vec3 m_TextColor;

		unsigned int m_Index;
		bool m_Clicked = false;
	};
}