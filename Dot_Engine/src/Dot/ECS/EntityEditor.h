#pragma once
#include "ECSManager.h"
#include "Dot/Core/Editor.h"

namespace Dot {
	
	class EntityEditor : public Editor
	{
	public:
		virtual void OnCreate();
		virtual void OnUpdate();
		virtual void OnEvent(Event& event);
		virtual void OnDestroy();

		Entity CreateNewEntity();
	private:
		Entity m_Entity;
	};
}