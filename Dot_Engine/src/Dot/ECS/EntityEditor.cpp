#include "stdafx.h"
#include "EntityEditor.h"



namespace Dot {
	void EntityEditor::OnCreate()
	{
		m_Entity = ECSManager::Get()->CreateEntity();

		
	}
	void EntityEditor::OnUpdate()
	{
	}
	void EntityEditor::OnEvent(Event& event)
	{
	}
	void EntityEditor::OnDestroy()
	{
	}
	Entity EntityEditor::CreateNewEntity()
	{
		return Entity();
	}
}