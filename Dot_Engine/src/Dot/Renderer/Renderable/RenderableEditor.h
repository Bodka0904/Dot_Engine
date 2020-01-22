#pragma once
#include "Dot/Core/Editor.h"
#include "Dot/Renderer/Renderable/Renderable.h"
#include "Dot/Renderer/Renderable/AnimatedMesh.h"
#include "Dot/Renderer/Renderable/ParticleMesh.h"


#include "Dot/Core/AssetManager.h"
namespace Dot {
	//struct GuiAnimation
	//{		
	//	GuiAnimation(Ref<AnimatedMesh> renderable)
	//		:mesh(renderable)
	//	{
	//		Wrapper::Create("Animation", glm::vec2(600, 400), glm::vec2(300, 200), 5);
	//		Gui::Get()->EnableWrapper("Animation");
	//		{
	//			TextArea::Create("Model", glm::vec2(0, 0), glm::vec2(200, 20), TextArea::TYPE::TEXT);			
	//			Button::Create("Save", glm::vec2(0, 0), glm::vec2(50, 50));
	//
	//		}Gui::Get()->DisableWrapper();
	//	}
	//	~GuiAnimation()
	//	{
	//		Gui::Get()->RemoveWrapper("Animation");
	//	}
	//	void Update()
	//	{
	//		if (Button::GetWrapped("Animation", "Save").GetClicked())
	//		{
	//			std::string model = TextArea::GetWrapped("Animation", "Model").GetValue<std::string>();
	//			mesh = AssetManager::Get()->GetCopyAnimMesh(model);
	//		}
	//	}
	//
	//	void OnEvent(Event& event)
	//	{
	//		if (event.GetEventType() == EventType::KeyPressed)
	//		{
	//			KeyPressedEvent& e = (KeyPressedEvent&)event;
	//			if (TextArea::GetWrapped("Animation", "Model").GetClicked())
	//			{
	//				TextArea::GetWrapped("Animation", "Model").TakeInput(e);
	//				e.IsHandled();
	//			}
	//		}
	//	}
	//	Ref<AnimatedMesh> mesh;
	//};
	//struct GuiParticle
	//{
	//	GuiParticle(Ref<ParticleMesh> renderable)
	//		:mesh(renderable)
	//	{
	//		Wrapper::Create("Particle", glm::vec2(600, 400), glm::vec2(300, 200), 5);
	//		Gui::Get()->EnableWrapper("Particle");
	//		{
	//			TextArea::Create("Model", glm::vec2(0, 0), glm::vec2(200, 20), TextArea::TYPE::TEXT);
	//			Button::Create("Save", glm::vec2(0, 0), glm::vec2(50, 50));
	//
	//		}Gui::Get()->DisableWrapper();
	//	}
	//	~GuiParticle()
	//	{
	//		Gui::Get()->RemoveWrapper("Particle");
	//	}
	//	void Update()
	//	{
	//		if (Button::GetWrapped("Particle", "Save").GetClicked())
	//		{
	//			std::string model = TextArea::GetWrapped("Particle", "Model").GetValue<std::string>();
	//			mesh =	std::make_shared<ParticleMesh>();
	//		}
	//	}
	//
	//	void OnEvent(Event& event)
	//	{
	//		if (event.GetEventType() == EventType::KeyPressed)
	//		{
	//			KeyPressedEvent& e = (KeyPressedEvent&)event;
	//			if (TextArea::GetWrapped("Particle", "Model").GetClicked())
	//			{
	//				TextArea::GetWrapped("Particle", "Model").TakeInput(e);
	//				e.IsHandled();
	//			}
	//		}
	//	}
	//	Ref<ParticleMesh> mesh;
	//};
	//struct GuiStaticMesh
	//{
	//	GuiStaticMesh(Ref<StaticMesh> renderable)
	//		:mesh(renderable)
	//	{
	//		Wrapper::Create("Static Mesh", glm::vec2(600, 400), glm::vec2(300, 200), 5);
	//		Gui::Get()->EnableWrapper("Static Mesh");
	//		{
	//			TextArea::Create("Model", glm::vec2(0, 0), glm::vec2(200, 20), TextArea::TYPE::TEXT);
	//			Button::Create("Save", glm::vec2(0, 0), glm::vec2(50, 50));
	//
	//		}Gui::Get()->DisableWrapper();
	//	}
	//	~GuiStaticMesh()
	//	{
	//		Gui::Get()->RemoveWrapper("Static Mesh");
	//	}
	//	void Update()
	//	{
	//		if (Button::GetWrapped("Static Mesh", "Save").GetClicked())
	//		{
	//			std::string model = TextArea::GetWrapped("Static Mesh", "Model").GetValue<std::string>();
	//			mesh = AssetManager::Get()->GetCopyStaticMesh(model);
	//		}
	//	}
	//
	//	void OnEvent(Event& event)
	//	{
	//		if (event.GetEventType() == EventType::KeyPressed)
	//		{
	//			KeyPressedEvent& e = (KeyPressedEvent&)event;
	//			if (TextArea::GetWrapped("Static Mesh", "Model").GetClicked())
	//			{
	//				TextArea::GetWrapped("Static Mesh", "Model").TakeInput(e);
	//				e.IsHandled();
	//			}
	//		}
	//	}
	//	Ref<StaticMesh> mesh;
	//};

	class RenderableEditor : public Editor
	{
	public:
		virtual void OnCreate(Entity entity);
		virtual void OnUpdate();
		virtual void OnEvent(Event& event);
		virtual void OnDestroy();
	
	private:
		Ref<Renderable> m_Renderable;
		Ref<Material> m_Material;

		//GuiAnimation* m_GuiAnim = NULL;
		//GuiParticle* m_GuiParticle = NULL;
		//GuiStaticMesh* m_GuiStaticMesh = NULL;
	};
}