#pragma once
#include "Widget.h"
#include "Dot/Renderer/Renderable/Renderable2D.h"
#include "Dot/Utils/Text/Text.h"

namespace Dot {
	class Dropdown : public Widget
	{
	public:
		Dropdown(const std::string& label, const glm::vec2& position, const glm::vec2& size, const glm::vec3& color);
		~Dropdown();
		virtual void Clean() override;
		virtual bool MouseHoover(const glm::vec2& mousePos)override;
		virtual bool OnLeftClick(const glm::vec2& mousePos) override;
		virtual void Move(const glm::vec2& pos)override;
		virtual void SetPosition(const glm::vec2& pos)override;
		virtual void SetColor(const glm::vec3& color)override;
		virtual void SetSize(const glm::vec2& size)override;
		virtual void StopRender()override;
		virtual const glm::vec2& GetSize();
		void AddBox(const std::string& text);

		const bool Clicked(int index);
		const bool Checked(int index);
		static Ref<Widget> Create(const std::string& label, const glm::vec2& position, const glm::vec2& size, const glm::vec3& color);
	private:
		void updateBuffers();
		void minimize();
		glm::vec4 getCoords();
	private:
		struct Dropbox
		{
			QuadVertex2D quad;
			Text text;
			int index;			
		};
		std::vector<Dropbox> m_Box;

		glm::vec2 m_Position;
		glm::vec2 m_Size;

		int m_CheckedBox;
		bool m_Minimized;

		static constexpr float offset = 1.0f;
	};
}