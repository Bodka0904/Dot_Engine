#pragma once
#include "Dot/Layer.h"
#include "Dot/Graphics/Gui/Gui.h"

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
		Ref<Gui> m_Stack;

		float value = 0.0f;
		
	};
}
