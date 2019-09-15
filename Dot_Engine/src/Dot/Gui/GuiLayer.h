#pragma once
#include "Dot/Layer.h"
#include "Dot/Graphics/Gui/WidgetStack.h"

namespace Dot {

	class GuiLayer : public Layer
	{
	public:
		GuiLayer();
		~GuiLayer();
		
		void OnAttach() override;
		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& event)override;
	private:
		Ref<WidgetStack> m_Stack;

		float value = 0.0f;
		
	};
}
