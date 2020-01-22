#pragma once
#include "Dot/Core/Events/Event.h"
#include "Dot/Renderer/Renderer2D.h"
#include "Dot/Renderer/Renderer2D.h"
#include "Widgets/Widget.h"
#include "Widgets/Panel.h"
#include "Widgets/Console.h"
#include "Widgets/GuiWindow.h"
#include "GuiLayout.h"


#include <queue>

namespace Dot {
	class GuiBlock
	{
		friend class GuiApplication;
	public:
		GuiBlock();
		virtual ~GuiBlock();
		
		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate() {};
		virtual void OnEvent(Event& event) {};
		virtual bool OnLeftClick(const glm::vec2& mousePos);
		virtual void OnRightClick();
		virtual void OnLeftRelease();
		virtual void SetLayout(GuiLayout& layout);
		
		void HandleResize(const glm::vec2& mousePos);
		void AddWidget(const std::string& name, const Ref<Widget> widget);
		void AddPanel(const std::string& name,Ref<Panel> panel);
		void AddConsole(const std::string& name, Ref<Console> console);
		void AddWindow(const std::string& name, Ref<GuiWindow> window);

		Ref<GuiWindow> GetWindow(const std::string& name)
		{
			return m_Window[name];
		}
		Ref<Console> GetConsole(const std::string& name)
		{
			return m_Console[name];
		}
		Ref<Panel>GetPanel(const std::string& name)
		{
			return m_Panel[name];
		}
		template<typename T>
		T& GetWidget(const std::string& name)
		{
			return *std::static_pointer_cast<T>(m_Widget[name]);
		}
	protected:	
		std::unordered_map<std::string, Ref<Widget>> m_Widget;
		std::unordered_map<std::string, Ref<Panel>> m_Panel;
		std::unordered_map<std::string, Ref<Console>> m_Console;
		std::unordered_map<std::string, Ref<GuiWindow>> m_Window;
		
		GuiLayout m_Layout;

	private:
		bool MouseHoover(glm::vec2& mousePos);
		glm::vec4 getCoords();

	private:
		bool m_ResizeLeft = false;
		bool m_ResizeRight = false;
	protected:
		
		struct LayoutBlock
		{
			enum
			{
				LEFT, MIDDLE, RIGHT, NUM
			};

			LayoutBlock() {};
			LayoutBlock(const glm::vec2& position, const glm::vec2& size);

			glm::vec2 Size;
			glm::vec2 Position;

			
			std::unordered_map<std::string, Ref<Panel>> Panel;
			std::unordered_map<std::string, Ref<Console>> Console;
			std::unordered_map<std::string, Ref<GuiWindow>> Window;

			glm::vec4 getCoords();
		};

		LayoutBlock m_LayoutBlock[LayoutBlock::NUM];
	};
}