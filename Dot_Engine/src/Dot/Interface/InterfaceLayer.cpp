#include "stdafx.h"
#include "InterfaceLayer.h"
#include "Dot/Core/SceneManager.h"
#include "Dot/Core/Input.h"


namespace Dot {
	InterfaceLayer::InterfaceLayer()
	{
	}
	void InterfaceLayer::OnAttach()
	{
	}
	void InterfaceLayer::OnDetach()
	{
	}
	void InterfaceLayer::OnUpdate(Timestep ts)
	{
	}
	void InterfaceLayer::OnEvent(Event& event)
	{
	}
	void InterfaceLayer::OnGuiAttach()
	{
	
	}
	void InterfaceLayer::OnGuiUpdate()
	{
	}
	void InterfaceLayer::OnGuiDetach()
	{
	}
	void InterfaceLayer::OnGuiEvent(Event& event)
	{
	}
	void InterfaceLayer::SetEditor(std::shared_ptr<Editor> editor)
	{
		m_CurrentEditor->OnDestroy();
		m_CurrentEditor = editor;
		//m_CurrentEditor->OnCreate();
	}
}