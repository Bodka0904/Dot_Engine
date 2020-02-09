#include "stdafx.h"
#include "BrushInstance.h"
#include "Dot/Core/AssetManager.h"



namespace Dot {
	BrushInstance::BrushInstance(const std::string& model, const std::string texture, int capacity)
		: m_Model(model),m_CurrentCount(0), m_CapacityPerInstance(capacity)
	{
		m_Texture = AssetManager::Get()->GetTexture(texture);
		
		// Requires to create own copy
		Ref<InstancedMesh> instance = std::make_shared<InstancedMesh>(model,capacity);
		m_Instance.push_back(instance);
	}
	void BrushInstance::Update(const std::vector<glm::mat4>& transforms)
	{
		if ((m_CurrentCount + transforms.size()) > m_CapacityPerInstance)
		{
			Ref<InstancedMesh> instance = std::make_shared<InstancedMesh>(m_Model, m_CapacityPerInstance);
			m_Instance.push_back(instance);
			m_Instance[m_Instance.size() - 1]->UpdateBuffer();
			m_CurrentCount = transforms.size();
		}
		else
		{
			m_Instance[m_Instance.size() - 1]->UpdateBuffer();
			m_CurrentCount += transforms.size();		
		}
	}
}