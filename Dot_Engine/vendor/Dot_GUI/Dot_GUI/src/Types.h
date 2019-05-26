#pragma once
#include "stdafx.h"

enum class WidgetType
{
	WINDOW,

	BUTTON
};

struct WidgetParams
{
	WidgetParams(WidgetType Type, int Width, int Height, std::string Name)
		: type(Type), width(Width), height(Height), name(Name)
	{};
	int width;
	int height;
	std::string name;
	WidgetType type;

};


struct WinGuiParams
{
	WinGuiParams(int Width,int Height,int PosX,int PosY)
		: width(Width),height(Height),posX(PosX),posY(PosY)
	{}
	int width;
	int height;
	int posX;
	int posY;
};