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

		void ButtonAction();
		void CheckBoxAction();
		void ArrowButtonAction();

		void TestVisibleOff();
		void TestVisibleOn();


	};
}
