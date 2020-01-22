#pragma once
#include "Dot/Core/Layer.h"
#include "Dot/Core/Editor.h"

namespace Dot {
	class InterfaceLayer : public Layer
	{
	public:
		InterfaceLayer();
		virtual void OnAttach() override;
		virtual void OnDetach()  override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& event) override;

		virtual void OnGuiAttach() override;
		virtual void OnGuiUpdate() override;
		virtual void OnGuiDetach() override;
		virtual void OnGuiEvent(Event& event) override;

		void SetEditor(std::shared_ptr<Editor> editor);
	private:
		std::shared_ptr<Editor> m_CurrentEditor;
	};

}