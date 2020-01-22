#include "stdafx.h"
#include "GuiLayout.h"

namespace Dot {
	GuiLayout::GuiLayout(const std::initializer_list<GuiElement>& elements)
		: Elements(elements)
	{
		glm::vec2 winSize = Input::GetWindowSize();
		glm::vec2 positionLeft(0, 0);
		glm::vec2 positionRight(winSize.x, 0);
		glm::vec2 positionBottom(0, winSize.y);

		float leftBorder;
		float rightBorder;

		bool LeftFull = false;
		bool RightFull = false;
		bool BottomFull = false;
		bool TopFull = false;

		for (auto& ele : Elements)
		{
			ele.Size *= winSize;
			if (ele.POS == ElementPosition::LEFT)
			{
				if (!LeftFull)
				{
					ele.Position = positionLeft;
					positionBottom.x = ele.Size.x + xOffset;
					positionLeft.y += ele.Size.y + yOffset;
					leftBorder = ele.Size.x;
					if (positionLeft.y > winSize.y)
					{
						ele.Size.y = winSize.y - ele.Position.y;
						positionLeft.y = 0;
						LeftFull = true;
					}
				}
				else
				{
					ele.TYPE = ElementType::NONE;
					LOG_WARN("Left layout is full!");
				}
			}
			else if (ele.POS == ElementPosition::RIGHT)
			{
				if (!RightFull)
				{
					ele.Position = glm::vec2(positionRight.x - ele.Size.x, positionRight.y);
					positionRight.y += ele.Size.y + yOffset;
					rightBorder = ele.Position.x;
					if (positionRight.y > winSize.y)
					{
						ele.Size.y = winSize.y - ele.Position.y;
						positionRight.y = 0;
						RightFull = true;
					}
				}
				else
				{
					ele.TYPE = ElementType::NONE;
					LOG_WARN("Right layout is full!");
				}
			}
		}
		for (auto& ele : Elements)
		{
			if (ele.POS == ElementPosition::BOTTOM)
			{
				if (!BottomFull)
				{
					ele.Position.x = positionBottom.x;
					ele.Position.y = positionBottom.y - ele.Size.y;
					positionBottom.x += ele.Size.x + xOffset;
					if (positionBottom.x > rightBorder)
					{
						ele.Size.x = rightBorder - ele.Position.x - xOffset;
						BottomFull = true;
					}
				}
				else
				{
					ele.TYPE = ElementType::NONE;
					LOG_WARN("Bottom layout is full!");
				}
			}
			else if (ele.POS == ElementPosition::TOP)
			{
				if (!BottomFull)
				{
				}
				else
				{
					ele.TYPE = ElementType::NONE;
					LOG_WARN("Top layout is full!");
				}
			}
		}

	}
	
}