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
		Wrapper::Create("Terrain Editor", glm::vec2(100, 100), glm::vec2(300, 600));
		Gui::EnableWrapper("Terrain Editor");
		{
			Arrbutton::Create("Amplitude", glm::vec2(100, 100), glm::vec2(50, 50));
			Arrbutton::Create("Roughness", glm::vec2(100, 100), glm::vec2(50, 50));
			Arrbutton::Create("Height", glm::vec2(100, 100), glm::vec2(50, 50));
			Arrbutton::Create("Seed", glm::vec2(100, 100), glm::vec2(50, 50));
			Button::Create("Process", glm::vec2(100, 100), glm::vec2(50, 50));
		
		}Gui::DisableWrapper();
	}

	static std::mutex s_Mutex;
	
	static void Update(Ref<Terrain> ter, float height,bool* processed)
	{
		std::lock_guard<std::mutex>lock(s_Mutex);
		ter->ApplyHeightsValueNoise(height);
		


		LOG_INFO("Finished");
		*processed = true;
	}
	
	void TerrainEditor::UpdateTerrain(Ref<Terrain> terrain)
	{
		if (Arrbutton::GetWrapped("Terrain Editor", "Seed").LeftClicked())
		{	
			m_Seed--;
			LOG_INFO("Seed: %d", m_Seed);
		}
		else if (Arrbutton::GetWrapped("Terrain Editor", "Seed").RightClicked())
		{
			m_Seed++;
			LOG_INFO("Seed: %d", m_Seed);
		}
		else if (Arrbutton::GetWrapped("Terrain Editor", "Amplitude").LeftClicked())
		{
			m_Amplitude -= 0.5f;
			LOG_INFO("Amplitude: %f", m_Amplitude);
		}
		else if (Arrbutton::GetWrapped("Terrain Editor", "Amplitude").RightClicked())
		{
			m_Amplitude += 0.5f;
			LOG_INFO("Amplitude: %f", m_Amplitude);
		}
		else if (Arrbutton::GetWrapped("Terrain Editor", "Height").RightClicked())
		{
			m_Height += 0.5f;
			LOG_INFO("Height: %f", m_Height);
		}
		else if (Arrbutton::GetWrapped("Terrain Editor", "Height").LeftClicked())
		{
			m_Height -= 0.5f;
			LOG_INFO("Height: %f", m_Height);
		}
		
		if (Button::GetWrapped("Terrain Editor","Process").GetClicked())
		{
			LOG_INFO("Processing...");
			terrain->SetSeed(m_Seed);
			terrain->SetAmplitude(m_Amplitude);
			terrain->SetRoughness(m_Roughness);
			m_Future = std::async(std::launch::async, Update, terrain,m_Height,&m_Processed);
		
		}
		if (m_Future._Is_ready())
		{
			if (m_Processed)
			{
				terrain->Update();
				terrain->ApplyNormals();
				LOG_INFO("Updated");
				m_Processed = false;
			}
		}
		
	}
	
}