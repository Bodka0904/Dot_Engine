#pragma once
#include "Widget.h"
#include "Dot/Renderer/Renderable/Renderable2D.h"
#include "Dot/Utils/Text/Text.h"

namespace Dot {
	class ArrowButton : public Widget
	{
	public:
		ArrowButton(const std::string& label, const glm::vec2& position, const glm::vec2& size, const glm::vec3& color);
		~ArrowButton();
		virtual bool OnLeftClick(const glm::vec2& mousePos) override;
		virtual bool MouseHoover(const glm::vec2& mousePos);
		virtual bool Release() override;
		virtual void Move(const glm::vec2& pos);
		virtual void SetPosition(const glm::vec2& pos);
		virtual void SetColor(const glm::vec3& color);
		virtual void SetSize(const glm::vec2& size);
		virtual void StopRender() override;
		virtual const glm::vec2& GetSize();

		const bool LeftClicked();
		const bool RightClicked();
		static Ref<Widget> Create(const std::string& label, const glm::vec2& position, const glm::vec2& size, const glm::vec3& color);
	private:
		void updateBuffers();
		glm::vec4 getCoords();
	private:
		struct Side
		{
			QuadVertex2D quad;
			glm::vec2 size;
			unsigned int index;
			bool clicked = false;
		};
		Side m_Left;
		Side m_Right;

		Text m_Label;
		Text m_Text;

		glm::vec2 m_Position;
		glm::vec2 m_Size;
	};
}