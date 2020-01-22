#pragma once
#include "Events/Event.h"
#include "Dot/ECS/Types.h"

namespace Dot {
	class Editor
	{
	public:
		virtual ~Editor() = default;
		virtual void OnCreate(Entity entity){};
		virtual void OnUpdate(){};
		virtual void OnEvent(Event& event){};
		virtual void OnDestroy(){};
	};
}