#pragma once
#include "Widget.h"
#include "Dot/Renderer/Renderable/Renderable2D.h"
#include "Dot/Utils/Text/Text.h"

namespace Dot {
	class CheckBox : public Widget
	{
	public:
		CheckBox(const std::string& label, const glm::vec2& position, const glm::vec2& size, const glm::vec3& color);
		~CheckBox();
		virtual void Clean() override;
		virtual bool MouseHoover(const glm::vec2& mousePos)override;
		virtual bool OnLeftClick(const glm::vec2& mousePos) override;
		virtual void Move(const glm::vec2& pos)override;
		virtual void SetPosition(const glm::vec2& pos)override;
		virtual void SetColor(const glm::vec3& color)override;
		virtual void SetSize(const glm::vec2& size)override;
		virtual void StopRender()override;
		virtual const glm::vec2& GetSize();
		const bool Clicked();
		static Ref<Widget> Create(const std::string& label, const glm::vec2& position, const glm::vec2& size, const glm::vec3& color);
	private:
		void updateBuffers();
		glm::vec4 getCoords();
	private:
		QuadVertex2D m_Quad;
		Text m_Label;
		Text m_Text;

		glm::vec2 m_Position;
		glm::vec2 m_Size;

		int m_Index;
		bool m_Clicked = false;

	};

}