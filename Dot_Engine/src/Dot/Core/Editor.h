#pragma once
#include "Events/Event.h"

namespace Dot {
	class Editor
	{
	public:
		virtual ~Editor() = default;
		virtual void OnCreate() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnEvent(Event& event) = 0;
		virtual void OnDestroy() = 0;
	};
}