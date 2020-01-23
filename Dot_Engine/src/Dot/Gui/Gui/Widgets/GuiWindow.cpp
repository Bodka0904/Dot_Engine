#include "stdafx.h"
#include "GuiWindow.h"
#include "Dot/Gui/Gui/GuiApplication.h"
#include "Dot/Core/Input.h"

namespace Dot {
	GuiWindow::GuiWindow(const glm::vec2& position, const glm::vec2& size, const glm::vec3& labelColor, const std::string& label)
		:
		m_Label("Arial", label, glm::vec2(position.x, position.y + 4), glm::vec2(0.2f), labelColor, MAX_CHAR_PER_LABEL),
		m_Position(position),
		m_Size(size)
	{
		m_FrameBuffer = Framebuffer::Create(Input::GetWindowSize().x, Input::GetWindowSize().y, FramebufferFormat::RGBA16F);
		glm::vec2 panelCoords[4] = {
					glm::vec2(0.0f ,	   1.0f / 8.0f),
					glm::vec2(1.0f / 8.0f, 1.0f / 8.0f),
					glm::vec2(1.0f / 8.0f, 2.0f / 8.0f),
					glm::vec2(0.0f ,	   2.0f / 8.0f)
				  
		};
		glm::vec2 coords[4] = {
			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f,1.0f),
			glm::vec2(1.0f,0.0f),
			glm::vec2(0.0f,0.0f)
				
		};

		m_Renderer = std::make_shared<Renderer2D>(1);

		m_PanelSize = glm::vec2(size.x, Font::GetFont("Arial")->GetData().lineHeight * 0.2f + 5);
		m_PanelQuad = QuadVertex2D(glm::vec2(position), m_PanelSize, glm::vec3(0.5, 0.5, 0.5), &panelCoords[0]);
		m_Quad = QuadVertex2D(glm::vec2(position.x,position.y + m_PanelSize.y),glm::vec2(size.x,size.y - m_PanelSize.y), glm::vec3(1, 1, 1), &coords[0]);

		m_Index = GuiApplication::Get()->PopIndex();

		updateBuffers();
	}
	GuiWindow::~GuiWindow()
	{
		GuiApplication::Get()->PushIndex(m_Index);
	}
	void GuiWindow::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;
		updateBuffers();
	}
	void GuiWindow::Move(const glm::vec2& pos)
	{
		m_Position += pos;
		updateBuffers();
	}
	void GuiWindow::Update(const glm::vec2& mousePos)
	{
		
	}
	void GuiWindow::Render()
	{
		m_FrameBuffer->BindTexture(0);
		m_Renderer->Render();
	}
	void GuiWindow::Set(float pos, float size)
	{
		m_Position.x = pos;
		m_Size.x = size;
		updateBuffers();
	}
	void GuiWindow::OnWindowResize(WindowResizeEvent& event)
	{
		m_FrameBuffer->Resize(event.GetWidth(),event.GetHeight());
	}
	Ref<GuiWindow> GuiWindow::Create(const glm::vec2& position, const glm::vec2& size, const glm::vec3& labelColor, const std::string& label)
	{
		Ref<GuiWindow> window = std::make_shared<GuiWindow>(position, size, labelColor, label);
		return window;
	}
	void GuiWindow::updateBuffers()
	{
		m_Label.SetPosition(glm::vec2(m_Position.x, m_Position.y + 4));
		m_Quad.SetPosition(glm::vec2(m_Position.x,m_Position.y + m_PanelSize.y), glm::vec2(m_Size.x, m_Size.y - m_PanelSize.y));
		m_PanelQuad.SetPosition(m_Position, glm::vec2(m_Size.x, Font::GetFont("Arial")->GetData().lineHeight * 0.2f + 5));

		GuiApplication::Get()->UpdateVertexBuffer(m_Index, &m_PanelQuad);
		GuiApplication::Get()->UpdateLabelBuffer(m_Index, m_Label.GetVertice(0), m_Label.GetNumChar());
		m_Renderer->PushOffset(&m_Quad, 1, 0);
	}
}