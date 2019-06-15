#pragma once
#include "Dot/Layer.h"
#include "Dot_GUI/src/Gui.h"
#include "Dot/Debug/Log.h"
#include "Dot/ColorCodes.h"


class GuiLayer : public Layer
{
public:
	GuiLayer();
	~GuiLayer();

	void OnAttach() override;
	void OnUpdate() override;

	void ButtonAction();
	void CheckBoxAction();
};

