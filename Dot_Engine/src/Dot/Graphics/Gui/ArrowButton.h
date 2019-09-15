#pragma once
#include "WidgetStack.h"
#include "Dot/Graphics/Transform.h"
#include "Dot/Graphics/Text/Text.h"

namespace Dot {
	class ArrowButton : public Widget
	{
	public:
		ArrowButton(const std::string& label, const glm::vec2& position, const glm::vec2& size);
		virtual ~ArrowButton() override;

		virtual void Render(const Ref<Shader>& shader) override;
		virtual void RenderLabel() override;
		virtual void SetPosition(const glm::vec2& pos) override;
		virtual void ClickHandle() override { *m_Clicked = !*m_Clicked; }
		virtual bool MouseHoover(const glm::vec2& mousePos) override;


		virtual const bool& LeftClicked() const override { return m_LeftClick; }
		virtual const bool& RightClicked() const override { return m_RightClick; }
		
		virtual const glm::vec2& GetPosition() override { return m_Transform.GetPos(); }

		static void Create(const std::string& label, const glm::vec2& position, const glm::vec2& size);
	private:
		glm::vec4 GetCoords();
		virtual const bool& GetClicked() const override { return *m_Clicked; }
	private:
		Ref<ArrayBuffer>m_VAO;
		Ref<Text>m_Label;

		Transform2D m_Transform;

		glm::vec2 m_Size;
		bool *m_Clicked;

		bool m_LeftClick = false;
		bool m_RightClick = false;
	};
}