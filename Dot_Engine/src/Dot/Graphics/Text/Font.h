#pragma once
#include "Dot/Graphics/Renderer/Texture.h"

namespace Dot {
	struct Character
	{
		int id = 0;
		double xTextureCoord = 0;
		double yTextureCoord = 0;
		double xMaxTextureCoord = 0;
		double yMaxTextureCoord =0;
		double xOffset = 0;
		double yOffset = 0;
		double sizeX = 0;
		double sizeY = 0;
		double xAdvance = 0;
	};
	struct FontMetaData
	{
		int count = 0;
		int base = 0;

		int scaleW = 0;
		int scaleH = 0;
		double lineHeight = 0.0f;
	};
	class Font
	{
	public:
		Font(const std::string& filepath, const std::string& texture);
		
		const Character &GetCharacter(char c) { return m_Characters[c]; }
		const FontMetaData& GetData() { return m_MetaData; }

		static void Bind(const std::string& name) { s_Fonts[name]->m_Texture.Bind(0); }
		static const Ref<Font>& GetFont(const std::string& name) { return s_Fonts[name]; }
		static void AddFont(const std::string& name, const std::string& filepath, const std::string& texture);
	private:
		std::unordered_map<char ,Character> m_Characters;
		FontMetaData m_MetaData;
		Texture m_Texture;

	private:
		static std::unordered_map<std::string, Ref<Font> > s_Fonts;
	};

}