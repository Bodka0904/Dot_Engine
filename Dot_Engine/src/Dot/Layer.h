#pragma once
#include "Events/Event.h"
#include "Dot/Utils/Timestep.h"
namespace Dot {
	class Layer
	{
	public:
		Layer();
		virtual ~Layer();

		virtual void OnAttach() {} //Init
		virtual void OnDetach() {} //Destroy

		virtual void OnUpdate(Timestep ts) {} //Update
		virtual void OnGuiRender() {}

		virtual void OnEvent(Event& event) {}

	};

}