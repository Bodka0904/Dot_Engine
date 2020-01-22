#include "stdafx.h"
#include "MaterialStack.h"



namespace Dot {
	Scope<MaterialStack> MaterialStack::s_Instance = NULL;
	void MaterialStack::Init()
	{
		if (s_Instance == NULL)
		{
			s_Instance = std::make_unique<MaterialStack>();
		}
	}
	MaterialStack::MaterialStack()
	{
		for (MaterialID id = 0; id < MAX_MATERIALS; ++id)
		{
			m_AvailableIDs.push(id);
		}
	}
	void MaterialStack::RegisterMaterial(const std::string& name, Ref<Material> material)
	{
		D_ASSERT(m_Material.size() < MAX_MATERIALS - 1, "Max number of materials is %d", MAX_MATERIALS);
		m_Lookup[name] = m_AvailableIDs.front();
		m_AvailableIDs.pop();
		m_Material[m_Lookup[name]] = material;
	}
	Ref<Material> MaterialStack::GetMaterial(const std::string& name)
	{
		return m_Material[m_Lookup[name]];
	}
	Ref<Material> MaterialStack::GetMaterial(MaterialID id)
	{
		return m_Material[id];
	}
	MaterialID MaterialStack::GetMaterialID(const std::string& name)
	{
		return m_Lookup[name];
	}
}