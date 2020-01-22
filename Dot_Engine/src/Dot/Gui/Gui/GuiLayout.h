#pragma once
#include "Widgets/Widget.h"
#include "Dot/Core/Input.h"
#include <glm/glm.hpp>

namespace Dot {
	enum class ElementPosition
	{
		LEFT, RIGHT, TOP, BOTTOM
	};
	enum class ElementType
	{
		NONE, PANEL, CONSOLE
	};
	struct GuiElement
	{
		GuiElement(ElementPosition pos, ElementType type, const glm::vec2& size, const std::string& name)
			:Size(size), Name(name), POS(pos), TYPE(type)
		{
		}
		std::string Name;
		glm::vec2 Size;
		glm::vec2 Position;
		ElementPosition POS;
		ElementType TYPE;
	};
	class GuiLayout
	{
	public:
		GuiLayout() {};
		GuiLayout(const std::initializer_list<GuiElement>& elements);

		std::vector<GuiElement> Elements;
	private:
		static constexpr float xOffset = 1.0f;
		static constexpr float yOffset = 1.0f;
	};


	struct Block
	{
		Block(int id,const glm::vec2& position, const glm::vec2& size)
			: Position(position), Size(size),ID(id)
		{
		}
		glm::vec2 Position;
		glm::vec2 Size;
		int ID;
	};

	enum class Position
	{
		LEFT,MIDDLE, RIGHT
	};
	enum class Type
	{
		NONE, PANEL, CONSOLE, WINDOW
	};
	struct Element
	{
		Element(int id,Type type,const std::string& name)
			: ID(id),m_Type(type),m_Name(name)
		{
		}

		int ID;
		Type m_Type;
		std::string m_Name;
	};
	
}