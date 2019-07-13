#include "stdafx.h"
#include "TerrainEditorLayer.h"
#include "Dot_GUI/src/Gui.h"

namespace Dot {

	#define BIND_FN(x) std::bind(&TerrainEditorLayer::x ,this)
	TerrainEditorLayer::TerrainEditorLayer()
	{
	}
	TerrainEditorLayer::~TerrainEditorLayer()
	{
	}
	void TerrainEditorLayer::OnAttach()
	{
		ter_editor.reset(new TerrainEditor());
		
		Gui::AddWrapper("Terrain Editor",wrap.terrain_editor_wrapper);
		
		Gui::GetWrapper(wrap.terrain_editor_wrapper).AddButton(BIND_FN(NewTerrain), "new terrain");
		Gui::GetWrapper(wrap.terrain_editor_wrapper).AddButton(BIND_FN(SetHeight), "set height");
		Gui::GetWrapper(wrap.terrain_editor_wrapper).AddButton(BIND_FN(SetNormals), "set normals");
		Gui::GetWrapper(wrap.terrain_editor_wrapper).AddArrowButton(BIND_FN(ChangeSize), "size");
		Gui::GetWrapper(wrap.terrain_editor_wrapper).AddArrowButton(BIND_FN(ChangeNumVertex), "num vertex");
		Gui::GetWrapper(wrap.terrain_editor_wrapper).AddArrowButton(BIND_FN(ChangeHeight), "height");
		Gui::GetWrapper(wrap.terrain_editor_wrapper).AddCheckBox(BIND_FN(RenderTerrain), "render");

	}
	void TerrainEditorLayer::OnUpdate()
	{
		
	}
	void TerrainEditorLayer::OnEvent(Event& event)
	{
	}
	void TerrainEditorLayer::OnDetach()
	{
	}
	void TerrainEditorLayer::NewTerrain()
	{
		ter_editor->GenFlatTerrain(size,numvertex);
	}
	void TerrainEditorLayer::SetHeight()
	{
		ter_editor->ApplyHeightsValueNoise(height);
	}
	void TerrainEditorLayer::SetNormals()
	{
		ter_editor->ApplyNormals();
	}
	void TerrainEditorLayer::RenderTerrain()
	{
		ter_editor->Draw();
	}
	void TerrainEditorLayer::ChangeSize()
	{
		
		if (Gui::GetWrapper(wrap.terrain_editor_wrapper).GetArrowButton(2).LeftClicked())
		{
			size += 1.0f;
			std::cout << size << std::endl;
			LOG_INFO("SIZE: %f", size);
		}
		else if (Gui::GetWrapper(wrap.terrain_editor_wrapper).GetArrowButton(2).RightClicked())
		{
			size -= 1.0f;
			LOG_INFO("SIZE: %f", size);
		}
	}
	void TerrainEditorLayer::ChangeNumVertex()
	{
		if (Gui::GetWrapper(wrap.terrain_editor_wrapper).GetArrowButton(1).LeftClicked())
		{
			numvertex += 1;
			LOG_INFO("NUMVERTEX: %d", numvertex);
		}
		else if (Gui::GetWrapper(wrap.terrain_editor_wrapper).GetArrowButton(1).RightClicked())
		{
			numvertex -= 1;
			LOG_INFO("NUMVERTEX: %d", numvertex);
		}
	}
	void TerrainEditorLayer::ChangeHeight()
	{
		if (Gui::GetWrapper(wrap.terrain_editor_wrapper).GetArrowButton(0).LeftClicked())
		{
			height += 1;
			LOG_INFO("HEIGHT: %f", height);
		}
		else if (Gui::GetWrapper(wrap.terrain_editor_wrapper).GetArrowButton(0).RightClicked())
		{
			height -= 1;
			LOG_INFO("HEIGHT: %f", height);
		}
	}
}