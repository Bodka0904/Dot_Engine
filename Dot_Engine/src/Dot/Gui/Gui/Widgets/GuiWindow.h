#pragma once
#include "Widget.h"
#include "Dot/Renderer/Buffers/FrameBuffer.h"
#include "Dot/Renderer/Renderable/Renderable2D.h"
#include "Dot/Renderer/Renderer2D.h"
#include "Dot/Utils/Text/Text.h"
#include "Dot/Core/Events/Event.h"
#include "Button.h"

namespace Dot {
	class GuiWindow
	{
	public:
		GuiWindow(const glm::vec2& position, const glm::vec2& size, const glm::vec3& labelColor, const std::string& label);
		~GuiWindow();
		void ActivateRenderTarget();
		void Clean();
		void SetPosition(const glm::vec2& pos);
		void Move(const glm::vec2& pos);
		void Update(const glm::vec2& mousePos);
		void Render();

		void SetX(float pos, float size);
		void SetY(float pos, float height);

		void OnWindowResize(WindowResizeEvent& event);
		static Ref<GuiWindow> Create(const glm::vec2& position, const glm::vec2& size, const glm::vec3& labelColor, const std::string& label);
	private:
		void updateBuffers();
	private:
		Ref<Framebuffer> m_FrameBuffer;
		Ref<Renderer2D> m_Renderer;
		QuadVertex2D m_Quad;
		QuadVertex2D m_PanelQuad;
		Text m_Label;

		glm::vec2 m_Position;
		glm::vec2 m_Size;
		glm::vec2 m_PanelSize;
		int m_Index;
	};
}