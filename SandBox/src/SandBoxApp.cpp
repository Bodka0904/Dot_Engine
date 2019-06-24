#pragma once
#include <Dot.h>


class TestLayer : public Layer
{
public:
	TestLayer()
		: Layer()
	{

	}

	void OnAttach()
	{
	
	}

	void OnUpdate() override
	{
		
	}

	void OnEvent(Event & event) override
	{
		
		if (event.GetEventType() == EventType::MouseButtonPressed)
		{
			MouseButtonPressEvent& e = (MouseButtonPressEvent&)event;
			if (e.GetButton() == D_MOUSE_BUTTON_LEFT)
			{
			
				LOG_INFO("Left button pressed")
			}
		}
		

	}



private:
	

};


class SandBox : public Application
{
public:
	SandBox()
	{
		PushLayer(new TestLayer());
	
	}

	~SandBox()
	{

	}

private:
	
};




Application* CreateApplication()
{
	return new SandBox();
}





void Test()
{
	printf("test");
}