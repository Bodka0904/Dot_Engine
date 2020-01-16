#include "stdafx.h"
#include "TerrainEditor.h"
#include "Dot/Gui/GuiSystem/Gui.h"
#include "Dot/Gui/GuiSystem/Slider.h"
#include "Dot/Gui/GuiSystem/Button.h"
#include "Dot/Gui/GuiSystem/Arrowbutton.h"

#include <glm/glm.hpp>



namespace Dot {
	TerrainEditor::TerrainEditor()
	{
		D_ASSERT(Gui::Get(), "Class require GUI inited");
		Wrapper::Create("Terrain Editor", glm::vec2(100, 100), glm::vec2(300, 600),5);
		Gui::Get()->EnableWrapper("Terrain Editor");
		{
			Arrbutton::Create("Amplitude", glm::vec2(100, 100), glm::vec2(50, 50));
			Arrbutton::Create("Roughness", glm::vec2(100, 100), glm::vec2(50, 50));
			Arrbutton::Create("Height", glm::vec2(100, 100), glm::vec2(50, 50));
			Arrbutton::Create("Seed", glm::vec2(100, 100), glm::vec2(50, 50));
			Button::Create("Process", glm::vec2(100, 100), glm::vec2(50, 50));
		
		}Gui::Get()->DisableWrapper();
	}

	static std::mutex s_Mutex;
	
	static void Update(Ref<Terrain> ter, bool* processed)
	{

		std::lock_guard<std::mutex>lock(s_Mutex);
		ter->HeightsValueNoise();
		
		LOG_INFO("Finished");
		*processed = true;
	}
	
	void TerrainEditor::UpdateTerrain(Ref<Terrain> terrain)
	{	
		//if (Button::GetWrapped("Terrain Editor","Process").GetClicked())
		//{
		//	LOG_INFO("Processing...");
		//	terrain->SetSeed(m_Seed);
		//	terrain->SetAmplitude(m_Amplitude);
		//	terrain->SetRoughness(m_Roughness);
		//	terrain->SetHeight(m_Height);
		//	terrain->m_Roughness = 5;
		//	m_Future = std::async(std::launch::async, Update, terrain,&m_Processed);
		//
		//}
		//if (m_Future._Is_ready())
		//{
		//	if (m_Processed)
		//	{
		//		terrain->ApplyHeights();
		//		terrain->ApplyNormals();
		//		LOG_INFO("Updated");
		//		m_Processed = false;
		//	}
		//}
	}	
}