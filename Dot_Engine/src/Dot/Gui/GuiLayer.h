#pragma once
#include "Dot/Layer.h"
#include "Dot_GUI/src/Gui.h"
#include "Dot/Log.h"

class GuiLayer : public Layer
{
public:
	GuiLayer();
	~GuiLayer();

	void OnAttach() override;
	void OnUpdate() override;

	void ButtonActions();

};

