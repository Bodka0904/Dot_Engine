#pragma once
#include "Dot/Core/Events/Event.h"
#include "Dot/Renderer/Renderer2D.h"
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
		virtual void OnDetach() {};
		virtual void OnUpdate() {};
		virtual void OnEvent(Event& event) {};


		virtual bool OnLeftClick(const glm::vec2& mousePos);
		virtual void OnLeftRelease();
		virtual void SetLayout(Layout& layout);

		void HandleResize(const glm::vec2& mousePos);
		void AddWidget(const std::string& name, const Ref<Widget> widget);
	

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