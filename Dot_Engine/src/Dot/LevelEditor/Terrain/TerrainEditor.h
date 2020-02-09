#pragma once
#include "Terrain.h"
#include "Dot/Renderer/Shader/Shader.h"
#include "Dot/Core/Editor.h"
#include "Dot/Utils/MousePicker.h"
#include "Dot/Renderer/Material.h"
#include "Dot/Gui/Gui/GuiBlock.h"

namespace Dot {
	

	class TerrainEditorUI : public GuiBlock
	{
	public:
		virtual void OnAttach() override;
		virtual void OnUpdate() override;
		virtual void OnEvent(Event& event) override;

	private:
		Entity m_Entity;
	};
	
}