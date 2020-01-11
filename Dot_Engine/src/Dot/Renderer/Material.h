#pragma once
#include "Shader/Shader.h"
#include "Texture.h"

namespace Dot {
	class Material
	{
	public:
		Material(const Ref<Shader>& shader);
		~Material();

		template<typename T>
		void Set(const std::string& name, const T& val)
		{
			auto uni = m_Shader->FindUniform(name);
			D_ASSERT(uni, "Material uniform does not exist");
			D_ASSERT(uni->offset + uni->size <= m_Shader->GetUniformSize(), "Material uniform buffer out of range");
			memcpy(m_Buffer + uni->offset, (unsigned char*)&val, uni->size);
			m_UpdatedValues.push_back(name);
		}
		void Set(const Ref<Texture2D>& texture)
		{
			m_Texture = texture;
		}
		void Set(int32_t renderFlags)
		{
			m_RenderFlags = renderFlags;
		}

		const uint32_t GetID() { return m_Shader->GetRendererID(); }
		const int32_t GetRenderFlag() { return m_RenderFlags; }

		void Bind();
		void Update();
		const Ref<Shader>& GetShader() { return m_Shader; }
		static Ref<Material> Create(const Ref<Shader>& shader);
	private:
		Ref<Shader>	   m_Shader;
		Ref<Texture2D> m_Texture;
		unsigned char* m_Buffer;

		std::vector<std::string> m_UpdatedValues;
		int32_t m_RenderFlags = 0;
	};
}