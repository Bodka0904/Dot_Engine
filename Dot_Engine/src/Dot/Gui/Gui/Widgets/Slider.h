#pragma once
#include "Widget.h"
#include "Dot/Renderer/Renderable/Renderable2D.h"
#include "Dot/Utils/Text/Text.h"

namespace Dot {
	class Slider : public Widget
	{
	public:
		Slider(const std::string& label, const glm::vec2& position, const glm::vec2& size, const glm::vec3& color,float* value,float rangeStart,float rangeEnd);
		~Slider();
		virtual bool OnLeftClick(const glm::vec2& mousePos) override;
		virtual bool MouseHoover(const glm::vec2& mousePos);
		virtual bool Release() override;
		virtual void Move(const glm::vec2& pos);
		virtual void SetPosition(const glm::vec2& pos);
		virtual void SetColor(const glm::vec3& color);
		virtual void SetSize(const glm::vec2& size);
		virtual void StopRender() override;
		void Active(const glm::vec2& mousePos);
		virtual const glm::vec2& GetSize();

		static Ref<Widget> Create(const std::string& label, const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, float* value, float rangeStart, float rangeEnd);
	private:
		void updateBuffers();
		glm::vec4 getCoords();
	private:
		struct Grab
		{
			QuadVertex2D quad;
			unsigned int index;
			bool grab = false;
			glm::vec2 position;
			glm::vec2 size;
		};
	private:
		QuadVertex2D m_Quad;
		Text m_Label;
		Text m_Text;
		Grab m_Grab;

		glm::vec2 m_Position;
		glm::vec2 m_Size;

		unsigned int m_Index;
		float* m_Value;
		float m_Start;
		float m_End;
	};
}