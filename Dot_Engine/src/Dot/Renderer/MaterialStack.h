#pragma once
#include "Material.h"
#include <set>

namespace Dot {
#define MAX_MATERIALS 100
	using MaterialID = int;

	class MaterialStack
	{
	public:
		static void Init();
		MaterialStack();
		void RegisterMaterial(const std::string& name,Ref<Material> material);
		Ref<Material> GetMaterial(const std::string& name);
		Ref<Material> GetMaterial(MaterialID id);
		MaterialID GetMaterialID(const std::string& name);

		std::unordered_map<std::string, int>& GetLookup() { return m_Lookup; }
		static Scope<MaterialStack>& Get() { return s_Instance; }
	private:
		std::unordered_map<MaterialID, Ref<Material>> m_Material;
		std::unordered_map<std::string, int > m_Lookup;
		std::queue<MaterialID> m_AvailableIDs{};

	private:
		static Scope<MaterialStack> s_Instance;
	};
}