#pragma once
#include <string>

class Layer
{
public:
	Layer();
	virtual ~Layer();

	virtual void OnAttach() {} //Init
	virtual void OnDetach() {} //Destroy

	virtual void OnUpdate() {} //Update
	virtual void ImGuiRender() {} 

};

