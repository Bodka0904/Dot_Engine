#pragma once
#include "Dot/Core/Layer.h"

#include "Terrain/Terrain.h"
#include "Water/Water.h"
#include "Dot/Utils/MousePicker.h"
#include "Terrain/BrushInstance.h"

#include "Dot/Renderer/Texture.h"

namespace Dot {
	class LevelEditorLayer : public Layer
	{
	public:	
		virtual void OnAttach();
		virtual void OnDetach();

		virtual void OnUpdate(Timestep ts);
		virtual void OnGuiRender();

		virtual void OnEvent(Event& event);

	private:
		Ref<Terrain> m_Terrain;
		Ref<Water> m_Water;
		Ref<MousePicker> m_Picker;

		Ref<Texture2D> m_TerrainTexture;


		Ref<BrushInstance>m_Grass;
	};
}