#pragma once
#include "Dot/Layer.h"

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
		float value = 0.0f;
		
	};
}
