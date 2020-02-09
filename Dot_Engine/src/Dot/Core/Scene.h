#pragma once
#include "Dot/ECS/Types.h"
#include "Dot/Gui/Gui/Widgets/GuiWindow.h"

namespace Dot {
	class Scene
	{
	public:
		virtual void OnCreate() = 0;
		virtual void OnDestroy() = 0;
		virtual void OnActivate() =0;
		virtual void OnDeactivate()= 0;
		virtual void OnUpdate(float dt) = 0;
		virtual void OnRender() = 0;
		virtual void AddEntity(Entity entity) = 0;
		virtual void RemoveEntity(Entity entity) = 0;
	protected:
		std::vector<Entity> m_Entities;
	};
}