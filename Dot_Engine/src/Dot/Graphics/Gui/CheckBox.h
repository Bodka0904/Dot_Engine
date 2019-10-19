#pragma once
#include "Gui.h"
#include "Dot/Graphics/Transform.h"
#include "Dot/Graphics/Text/Text.h"

namespace Dot {
	class CheckBox : public Widget
	{
	public:
		CheckBox(const std::string& label, const glm::vec2& position, const glm::vec2& size);
		virtual ~CheckBox() override;

	
		virtual void Update(const Ref<Shader>& shader) override;
		virtual void RenderLabel() override;
		virtual void SetPosition(const glm::vec2& pos) override;
		virtual void SetIndex(const unsigned int index) { m_Index = index; };
		virtual void ClickHandle() override;
		virtual bool MouseHoover(const glm::vec2& mousePos) override;

		virtual const bool& GetClicked() const override { return m_Clicked; }
		virtual const glm::vec2& GetPosition() override { return m_Transform.GetPos(); }

		static void Create(const std::string& label, const glm::vec2& position, const glm::vec2& size);
	private:
		glm::vec4 GetCoords();
	private:
		virtual const bool& LeftClicked() const override { return m_Clicked; }
		virtual const bool& RightClicked() const override { return m_Clicked; }
	private:
		Ref<Text>m_Label;
		Transform2D m_Transform;

		glm::vec2 m_Size;
		bool m_Clicked = false;
		int m_TexOffset = 0;
		unsigned int m_Index = 0;
	};
}