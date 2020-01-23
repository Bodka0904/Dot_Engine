#pragma once
#include <glm/glm.hpp>
#include "Widgets/Panel.h"
#include "Widgets/Console.h"
#include "Widgets/GuiWindow.h"

namespace Dot {

	enum class ElementType
	{
		NONE,PANEL,CONSOLE,WINDOW
	};

	struct Element
	{
		Element(ElementType Type,float Height,const std::string& Name)
			:type(Type),height(Height),name(Name)
		{}

		std::string name;
		float height;
		ElementType type;
	};

	struct SubLayout
	{
		SubLayout(const glm::vec2& Position, const glm::vec2& Size, const std::initializer_list<Element>& Elements);

		glm::vec2 position;
		glm::vec2 size;
		std::vector<Element> elements;

		std::vector<Panel*> m_Panel;
		std::vector<GuiWindow*> m_Window;
		std::vector<Console*> m_Console;

		bool MouseHoover(const glm::vec2 mousePos);
		glm::vec4 getCoords();
	};

	class Layout
	{
	public:
		Layout() {};
		Layout(const std::initializer_list<SubLayout>& layout);
		
		void HandleResize(const glm::vec2& mousePos);
		void OnLeftClick(const glm::vec2& mousePos);
		void OnLeftRelease();
		std::vector<SubLayout> m_Layout;
	private:
		int m_ResizeIndex = -1;
		struct cmp
		{
			cmp() {};
			bool operator()(SubLayout a, SubLayout b)
			{
				return (a.position.x < b.position.x);
			}
		};
		cmp m_Cmp;
	};
}