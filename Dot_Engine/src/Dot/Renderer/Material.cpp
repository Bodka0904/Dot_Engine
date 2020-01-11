#include "stdafx.h"
#include "Material.h"
#include "Dot/Core/AssetManager.h"

namespace Dot {
	Material::Material(const Ref<Shader>& shader)
	{
		m_Shader = shader;
		m_Buffer = new unsigned char[shader->GetUniformSize()];
		m_Texture = AssetManager::Get()->GetTexture("Default");
	}
	Material::~Material()
	{
		delete[] m_Buffer;
	}
	void Material::Bind()
	{
		m_Shader->Bind();	
	}
	void Material::Update()
	{
		m_Texture->Bind(0);
		for (auto i = 0; i < m_UpdatedValues.size(); ++i)
		{
			m_Shader->SetUniform(m_UpdatedValues[i], m_Buffer);
		}
		m_UpdatedValues.clear();
	}
	Ref<Material> Material::Create(const Ref<Shader>& shader)
	{
		return std::make_shared<Material>(shader);
	}
}