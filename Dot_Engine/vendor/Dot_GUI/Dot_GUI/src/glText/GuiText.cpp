#include "GuiText.h"



glm::vec2 GuiText::m_windowSize = glm::vec2(1200, 720);

GuiText::GuiText(const std::string & rendText,const glm::vec2& offset,const glm::vec2& scale)
	:m_text(rendText),m_offset(offset),m_scale(scale)
{
	text = gltCreateText();
	gltSetText(text, m_text.c_str());
	gltViewport(m_windowSize.x, m_windowSize.y);
	
}


GuiText::~GuiText()
{
	gltDeleteText(text);
	gltTerminate();
}

void GuiText::Init()
{
	gltInit();
}

void GuiText::StartDraw()
{
	gltBeginDraw();
}

void GuiText::SetData(glm::vec2 pos)
{
	m_position = glm::vec2(pos.x + m_offset.x, pos.y + m_offset.y);
	m_scale = glm::vec2(m_scale.x, m_scale.y);
}

void GuiText::Draw()
{
	gltColor(1.0f, 1.0f, 1.0f, 1.0f);
	gltDrawText2D(text, 0, 0);
	
}

void GuiText::UpdateData(GuiTransform& transform)
{
	transform.SetScale(m_scale);
	transform.SetPos(m_position);

	gltUpdateShader(transform);
}


void GuiText::UpdateViewPort(int width, int height)
{
	gltViewport(width, height);
	m_windowSize.x = width;
	m_windowSize.y = height;
}
