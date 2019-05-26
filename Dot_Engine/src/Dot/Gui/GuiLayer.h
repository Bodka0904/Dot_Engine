#pragma once
#include "stdafx.h"
#include "Dot/Layer.h"
#include "Dot/Log.h"
#include "Dot_GUI/src/WinGui.h"

class GuiLayer : public Layer
{
public:
	GuiLayer();
	~GuiLayer();

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate() override;
	void OnGuiRender() override;

	
private:
	std::unique_ptr<WinGui> m_Gui;

};

