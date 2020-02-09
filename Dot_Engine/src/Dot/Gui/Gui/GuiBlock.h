#pragma once
#include "Dot/Core/Events/Event.h"
#include "Dot/Renderer/Renderer2D.h"
#include "Widgets/Widget.h"
#include "Widgets/Panel.h"
#include "Widgets/Console.h"
#include "Widgets/GuiWindow.h"
#include "Layout.h"


#include <queue>

namespace Dot {
	class GuiBlock
	{
		friend class GuiApplication;
	public:
		GuiBlock();
		virtual ~GuiBlock();
		
		virtual void OnAttach() {};
		virtual void OnUpdate() {};
		virtual void OnEvent(Event& event) {};
		virtual void OnRender() {};
	
		void OnDetach();
		void RestartLayout();
		void SetLayout(const Layout& layout);	
		void AddWidget(const std::string& name, const Ref<Widget> widget);
		void AddPanel(const std::string& name, const Ref<Panel> panel);
		void AddConsole(const std::string& name, const Ref<Console>console);
		void AddWindow(const std::string& name, const Ref<GuiWindow>window);

	private:
		void HandleResize(const glm::vec2& mousePos);
		bool OnLeftClick(const glm::vec2& mousePos);
		bool OnLeftRelease();
		void OnWindowResize();

	public:
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

		Layout m_Layout;
	};
}