#pragma once
#include "Dot/Log.h"


enum class EventType
{
	None = 0,

	KeyPressed,
	KeyReleased,
	
	MouseButtonPressed,
	MouseButtonReleased,

	MouseMoved
};

class Event
{
public:

	bool Handled = false;

	inline EventType &GetEventType() { return m_event; }
	inline int &GetKeyCode() { return m_keyCode; }
	inline int &GetMouseButton() { return m_mouseButton; }
	inline float &GetMouseX() { return m_mouseX; }
	inline float &GetMouseY() { return m_mouseY; }
	
	
private:
	EventType m_event;
	int m_keyCode;
	int m_mouseButton;
	
	float m_mouseX;
	float m_mouseY;
	
	
};




