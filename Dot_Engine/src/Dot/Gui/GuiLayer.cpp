#include "stdafx.h"
#include "GuiLayer.h"


GuiLayer::GuiLayer()
	:Layer()
{
}


GuiLayer::~GuiLayer()
{
}

void GuiLayer::OnAttach()
{
	m_Gui = std::unique_ptr<WinGui>(WinGui::Create());
}

void GuiLayer::OnDetach()
{
	
}

void GuiLayer::OnUpdate()
{
	m_Gui->Attach();
}

void GuiLayer::OnGuiRender()
{
}
