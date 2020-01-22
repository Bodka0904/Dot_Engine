#pragma once
#include "Dot/Gui/Gui/GuiLayout.h"
#include "Dot/Utils/Text/Text.h"
#include "Widget.h"

#include <glm/glm.hpp>

namespace Dot {
	class Panel
	{
	public:
		Panel(const glm::vec2& position, const glm::vec2& size, const glm::vec3& labelColor, const std::string& label, ElementPosition pos);
		~Panel();
		void Move(const glm::vec2& pos);
		bool Release() { return false; };
		void SetPosition(const glm::vec2& pos);
		void SetColor(const glm::vec3& color);
		void SetSize(const glm::vec2& size);
		void AddWidget(const std::string& name, Ref<Widget>widget);
		void Update(const glm::vec2& mousePos);
	
		bool OnLeftClick(const glm::vec2& mousePos);
		bool OnRightClick(const glm::vec2& mousePos);
		bool OnRelease();


		void Set(const glm::vec2& pos,const glm::vec2& size);
	

		template<typename T>
		T& GetWidget(const std::string& name)
		{
			return *std::static_pointer_cast<T>(m_Widget[name]);
		}

		static Ref<Panel> Create(const glm::vec2& position, const glm::vec2& size, const glm::vec3& labelColor, const std::string& label, ElementPosition pos);
	private:
		void updateBuffers();
		void handleResize();
		glm::vec4 getCoords();
	private:
		ElementPosition m_Pos;
		std::map<std::string, Ref<Widget>> m_Widget;

		unsigned int m_PanelIndex;
		unsigned int m_Index;

		Text m_Label;
		QuadVertex2D m_PanelQuad;
		QuadVertex2D m_Quad;

		glm::vec2 m_Position;
		glm::vec2 m_Size;
		glm::vec2 m_MaxSize;
		glm::vec2 m_PanelSize;

		bool m_Clicked = false;
	private:
		float xOffset = 10;
		float yOffset;
		float rowSize = 0.0f;
		int rowCount = 0;
	};

}