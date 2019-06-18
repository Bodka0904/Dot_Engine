#include "GuiText.h"


glm::vec2 GuiText::m_windowSize = glm::vec2(1200, 720);

GuiText::GuiText(const std::string & rendText)
	:m_text(rendText)
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
	m_position = glm::vec2(pos.x - 0.05, pos.y + 0.1);
	m_rotation = glm::vec2(3.14, 0.8);
	m_scale = glm::vec2(0.0025, 0.0025);
}

void GuiText::Draw()
{
	gltColor(0.5f, 0.0f, 0.5f, 1.0f);
	gltDrawText2D(text, -35, -40, 1);
	
}

void GuiText::UpdateData(GuiTransform& transform)
{
	transform.SetScale(m_scale);
	transform.SetRot(m_rotation);
	transform.SetPos(m_position);

	gltUpdateShader(transform);
}


void GuiText::UpdateViewPort(int width, int height)
{
	gltViewport(width, height);
	m_windowSize.x = width;
	m_windowSize.y = height;
}
