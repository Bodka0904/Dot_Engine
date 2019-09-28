#pragma once
#include "WidgetStack.h"
#include "Dot/Graphics/Transform.h"
#include "Dot/Graphics/Text/Text.h"

namespace Dot {
	class Slider : public Widget
	{
	public:
		Slider(const std::string& label, const glm::vec2& position, const glm::vec2& size,float* value);
		virtual ~Slider() override;

		virtual void Update(const Ref<Shader>& shader) override;
		virtual void RenderLabel() override;
		virtual void SetPosition(const glm::vec2& pos) override;
		virtual void SetIndex(const unsigned int index) { m_Index = index; };
		virtual void ClickHandle() override;
		virtual bool MouseHoover(const glm::vec2& mousePos) override;
	
		virtual const glm::vec2& GetPosition() override { return m_Transform.GetPos(); }

		static void Create(const std::string& label, const glm::vec2& position, const glm::vec2& size,float*value);
	private:
		glm::vec4 GetCoords();
		virtual const bool& GetClicked() const override { return m_Clicked; }
		virtual const bool& LeftClicked() const override { return m_Clicked; }
		virtual const bool& RightClicked() const override { return m_Clicked; }

	private:
		Ref<Text>m_Label;
		Transform2D m_Transform;

		glm::vec2 m_Size;
		bool m_Clicked = false;
		float m_TexOffset = 0.0f;
		
		float *m_Value;
		float m_TempStorage;

		unsigned int m_Index = 0;
	};
}