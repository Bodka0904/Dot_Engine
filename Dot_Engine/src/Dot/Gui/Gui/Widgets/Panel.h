#pragma once
#include "Dot/Utils/Text/Text.h"
#include "Widget.h"

#include <glm/glm.hpp>

namespace Dot {
	class Panel
	{
	public:
		Panel(const glm::vec2& position, const glm::vec2& size, const glm::vec3& labelColor, const std::string& label);
		~Panel();
		void Move(const glm::vec2& pos);
		void SetPosition(const glm::vec2& pos);
		void SetColor(const glm::vec3& color);
		void AddWidget(const std::string& name, Ref<Widget>widget);
	
		bool OnLeftClick(const glm::vec2& mousePos);
		bool OnRelease();


		void Set(float pos,float size);

		template<typename T>
		T& GetWidget(const std::string& name)
		{
			return *std::static_pointer_cast<T>(m_Widget[name]);
		}

		static Ref<Panel> Create(const glm::vec2& position, const glm::vec2& size, const glm::vec3& labelColor, const std::string& label);
	private:
		void updateBuffers();
		void handleResize();
		glm::vec4 getCoords();
	private:
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

	private:
		float xOffset = 10;
		float yOffset;
		float rowSize = 0.0f;
		int rowCount = 0;
	};

}