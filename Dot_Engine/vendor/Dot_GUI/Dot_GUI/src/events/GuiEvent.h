#pragma once
#include "stdafx.h"


enum class GuiEventType
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



class GuiEvent
{
public:
	virtual GuiEventType GetEventType() const = 0;

private:
	GuiEventType type = GuiEventType::None;

};



class GuiWindowResizeEvent : public GuiEvent
{
public:
	GuiWindowResizeEvent(int width, int height)
		:m_width(width), m_height(height)
	{}


	virtual GuiEventType GetEventType() const override { return type; }
	virtual std::pair<int, int> GetWinSize() const { return { (int)m_width, (int)m_height }; }
	inline int GetWidth() const { return m_width; }
	inline int GetHeight() const { return m_height; }

private:
	int m_width;
	int m_height;
	GuiEventType type = GuiEventType::WindowResized;

};


class GuiKeyPressedEvent : public GuiEvent
{
public:
	GuiKeyPressedEvent(int key)
		:m_key(key)
	{}
	virtual GuiEventType GetEventType() const override { return type; }
	virtual int GetKey() const { return m_key; }

private:
	int m_key;
	GuiEventType type = GuiEventType::KeyPressed;
};



class GuiKeyReleasedEvent : public GuiEvent
{
public:
	GuiKeyReleasedEvent(int key)
		:m_key(key)
	{}
	virtual GuiEventType GetEventType() const override { return type; }
	virtual int GetKey() const { return m_key; }

private:
	int m_key;
	GuiEventType type = GuiEventType::KeyReleased;
};




class GuiMouseButtonPressEvent : public GuiEvent
{
public:
	GuiMouseButtonPressEvent(int button)
		:m_button(button)
	{};


	virtual GuiEventType GetEventType() const override { return type; }
	inline int GetButton() const { return m_button; }

private:
	int m_button;
	GuiEventType type = GuiEventType::MouseButtonPressed;
};




class GuiMouseButtonReleaseEvent : public GuiEvent
{
public:
	GuiMouseButtonReleaseEvent(int button)
		:m_button(button)
	{};

	virtual GuiEventType GetEventType() const override { return type; }
	inline int GetButton() const { return m_button; }

private:
	int m_button;
	GuiEventType type = GuiEventType::MouseButtonReleased;
};

