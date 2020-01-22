#include "stdafx.h"
#include "RenderableEditor.h"

#include "Dot/Core/Input.h"
#include "Dot/Renderer/RenderSystem.h"

namespace Dot {
	void RenderableEditor::OnCreate(Entity entity)
	{
		//Wrapper::Create("Renderable Type", glm::vec2(400, 400), glm::vec2(200, 300), 4);
		//Gui::Get()->EnableWrapper("Renderable Type");
		//{
		//	Button::Create("Particle", glm::vec2(0, 0), glm::vec2(50, 50));
		//	Button::Create("Animated", glm::vec2(0, 0), glm::vec2(50, 50));
		//	Button::Create("Static", glm::vec2(0, 0), glm::vec2(50, 50));
		//	Button::Create("Instanced", glm::vec2(0, 0), glm::vec2(50, 50));
		//
		//}Gui::Get()->DisableWrapper();
		//
		//Wrapper::Create("Material", glm::vec2(600, 700), glm::vec2(200, 300), 4);
		//Gui::Get()->EnableWrapper("Material");
		//{
		//	Button::Create("New Material", glm::vec2(0, 0), glm::vec2(50, 50));
		//	Button::Create("Load Material", glm::vec2(0, 0), glm::vec2(50, 50));
		//
		//}Gui::Get()->DisableWrapper();

	}
	void RenderableEditor::OnUpdate()
	{
		//if (Button::GetWrapped("Renderable Type", "Particle").GetClicked())
		//{
		//	delete m_GuiAnim;
		//	m_GuiAnim = NULL;
		//	delete m_GuiStaticMesh;
		//	m_GuiStaticMesh = NULL;
		//	if (!m_GuiParticle)
		//	{
		//		m_GuiParticle = new GuiParticle(std::static_pointer_cast<ParticleMesh>(m_Renderable));
		//	}
		//}
		//else if (Button::GetWrapped("Renderable Type", "Animated").GetClicked())
		//{
		//	delete m_GuiParticle;
		//	m_GuiParticle = NULL;	
		//	delete m_GuiStaticMesh;
		//	m_GuiStaticMesh = NULL;
		//	if (!m_GuiAnim)
		//	{
		//		m_GuiAnim = new GuiAnimation(std::static_pointer_cast<AnimatedMesh>(m_Renderable));
		//	}	
		//}
		//else if (Button::GetWrapped("Renderable Type", "Static").GetClicked())
		//{
		//	delete m_GuiParticle;
		//	m_GuiParticle = NULL;
		//	delete m_GuiAnim;
		//	m_GuiAnim = NULL;
		//
		//	if (!m_GuiStaticMesh)
		//	{
		//		m_GuiStaticMesh = new GuiStaticMesh(std::static_pointer_cast<StaticMesh>(m_Renderable));
		//	}
		//}
		//else if (Button::GetWrapped("Renderable Type", "Instanced").GetClicked())
		//{
		//	delete m_GuiParticle;
		//	m_GuiParticle = NULL;
		//	delete m_GuiAnim;
		//	m_GuiAnim = NULL;
		//	delete m_GuiStaticMesh;
		//	m_GuiStaticMesh = NULL;
		//}
		//
		//if (m_GuiAnim)
		//{
		//	m_GuiAnim->Update();
		//}
		//else if (m_GuiParticle)
		//{
		//	m_GuiParticle->Update();
		//}
		//else if (m_GuiStaticMesh)
		//{
		//	m_GuiStaticMesh->Update();
		//}
	}
	void RenderableEditor::OnEvent(Event& event)
	{
		//if (m_GuiAnim)
		//{
		//	m_GuiAnim->OnEvent(event);
		//}
		//else if (m_GuiParticle)
		//{
		//	m_GuiParticle->OnEvent(event);
		//}
		//else if (m_GuiStaticMesh)
		//{
		//	m_GuiStaticMesh->OnEvent(event);
		//}
	}
	void RenderableEditor::OnDestroy()
	{
	}
}