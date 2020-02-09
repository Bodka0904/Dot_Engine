#pragma once
#include "Dot/Gui/Gui/GuiApplication.h"
#include "Dot/Gui/Gui/GuiBlock.h"
#include "Dot/Gui/Gui/Widgets/Button.h"
#include "Dot/Gui/Gui/Widgets/ArrowButton.h"
#include "Dot/Gui/Gui/Widgets/CheckBox.h"
#include "Dot/Gui/Gui/Widgets/Slider.h"
#include "Dot/Gui/Gui/Widgets/TextArea.h"
#include "Dot/Gui/Gui/Widgets/Console.h"
#include "Dot/Gui/Gui/Widgets/Panel.h"
#include "Dot/Gui/Gui/Widgets/GuiWindow.h"
#include "Dot/Gui/Gui/Widgets/Dropdown.h"
#include "Dot/Gui/Gui/Layout.h"
#include "Terrain/TerrainEditor.h"
#include "Dot/Renderer/RenderSystem.h"
#include "Dot/Renderer/Camera/CameraController.h"
#include "Dot/Renderer/Light/Light.h"
#include "Dot/Renderer/Shader/Shader.h"
#include "Dot/Core/SceneManager.h"
namespace Dot {

	class LevelEditor
	{
	public:
		class DefaultUI : public GuiBlock
		{
		public:
			virtual void OnAttach() override;
			virtual void OnUpdate() override;
			virtual void OnEvent(Event& event) override;
			virtual void OnRender() override;

			float m_Value = 0.0f;
			int m_TerrainEditorID;


			Ref<RenderSystem> m_RenderSystem;
			Ref<CameraController> m_CamController;
			Ref<Light> m_Light;
			Ref<Shader> m_StaticShader;
		};
		friend class DefaultUI;
	public:
		LevelEditor();
		~LevelEditor();

		void OnUpdate(float ts);
		void OnRender();

	private:
		int m_DefaultID;
		

		Ref<DefaultUI> m_Default;
		Ref<TerrainEditorUI> m_TerrainEditor;
	};
}