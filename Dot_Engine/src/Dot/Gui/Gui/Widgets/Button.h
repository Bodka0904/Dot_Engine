#pragma once
#include "Widget.h"
#include "Dot/Renderer/Renderable/Renderable2D.h"
#include "Dot/Utils/Text/Text.h"

namespace Dot {
	class Button : public Widget
	{
	public:
		Button(const std::string& label,const glm::vec2& position,const glm::vec2& size,const glm::vec3& color);
		~Button();
		virtual bool OnLeftClick(const glm::vec2& mousePos) override;
		virtual bool MouseHoover(const glm::vec2& mousePos);
		virtual bool Release() override;
		virtual void Move(const glm::vec2& pos);
		virtual void SetPosition(const glm::vec2& pos);
		virtual void SetColor(const glm::vec3& color);
		virtual void SetSize(const glm::vec2& size);
		virtual void StopRender() override;
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

		unsigned int m_Index;
		bool m_Clicked = false;
	};
}