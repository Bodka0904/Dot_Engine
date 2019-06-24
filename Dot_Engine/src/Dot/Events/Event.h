#pragma once 
#include "Dot/Debug/Log.h"
#include <list>

enum class EventType
{
	None = 0,

	WindowClosed,
	WindowResized,

	KeyPressed,
	KeyReleased,
	
	MouseButtonPressed,
	MouseButtonReleased,

	MouseMoved
};



class Event
{
public:
	virtual EventType GetEventType() const = 0;

private:
	EventType type = EventType::None;
	
};



class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(int width, int height)
		:m_width(width), m_height(height)
	{}

	
	virtual EventType GetEventType() const override { return type; }
	virtual std::pair<int, int> GetWinSize() const { return { (int)m_width, (int)m_height }; }
	inline int GetWidth() const { return m_width; }
	inline int GetHeight() const { return m_height; }

private:
	int m_width;
	int m_height;
	EventType type = EventType::WindowResized;

};


class KeyPressedEvent : public Event
{
public:
	KeyPressedEvent(int key)
		:m_key(key)
	{}
	virtual EventType GetEventType() const override { return type; }
	virtual int GetKey() const { return m_key; }

private:
	int m_key;
	EventType type = EventType::KeyPressed;
};



class KeyReleasedEvent : public Event
{
public:
	KeyReleasedEvent(int key)
		:m_key(key)
	{}
	virtual EventType GetEventType() const override { return type; }
	virtual int GetKey() const { return m_key; }

private:
	int m_key;
	EventType type = EventType::KeyReleased;
};




class MouseButtonPressEvent : public Event
{
public:
	MouseButtonPressEvent(int button)
		:m_button(button)
	{};

	
	virtual EventType GetEventType() const override { return type; }
	inline int GetButton() const { return m_button; }

private:
	int m_button;
	EventType type = EventType::MouseButtonPressed;
};




class MouseButtonReleaseEvent : public Event
{
public:
	MouseButtonReleaseEvent(int button)
		:m_button(button)
	{};

	virtual EventType GetEventType() const override { return type; }
	inline int GetButton() const { return m_button; }

private:
	int m_button;
	EventType type = EventType::MouseButtonReleased;
};

