#pragma once
#include "Events/Event.h"

class Layer
{
public:
	Layer();
	virtual ~Layer();

	virtual void OnAttach() {} //Init
	virtual void OnDetach() {} //Destroy

	virtual void OnUpdate() {} //Update
	virtual void OnGuiRender() {} 

	virtual void OnEvent(Event& event) {}

};

