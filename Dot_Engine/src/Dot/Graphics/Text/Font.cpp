#pragma once
#include "stdafx.h"
#include "Font.h"
#include <fstream>

namespace Dot {
	std::unordered_map<std::string, Ref<Font> > Font::s_Fonts;

	Font::Font(const std::string& filepath, const std::string& texture)
		: m_Texture(texture,false)
	{
		m_MetaData.paddingWidth = 6;
		m_MetaData.paddingHeight = 6;
		m_MetaData.scaleH = 1024;
		m_MetaData.scaleW = 1024;

		std::string line;
		std::ifstream FILE(filepath);
		std::vector<Character> test;
		if (FILE.is_open())
		{
			while (std::getline(FILE, line).good())
			{
				Character tmp;
				int correct = sscanf(line.c_str(),
				"char id=%d x=%lf y=%lf width=%lf height=%lf xoffset=%lf yoffset=%lf xadvance=%lf",
				&tmp.id, &tmp.xTextureCoord, &tmp.yTextureCoord, &tmp.sizeX, &tmp.sizeY, &tmp.xOffset, &tmp.yOffset, &tmp.xAdvance);
	
				if (correct != 8)
					break;
		
				tmp.xTextureCoord /= m_MetaData.scaleW;
				tmp.yTextureCoord /= m_MetaData.scaleH;
				tmp.xMaxTextureCoord = tmp.xTextureCoord + (tmp.sizeX / m_MetaData.scaleW);
				tmp.yMaxTextureCoord = tmp.yTextureCoord + (tmp.sizeY / m_MetaData.scaleH);
				m_Characters[tmp.id] = tmp;
			}
	
			FILE.close();
		}
	}
	void Font::AddFont(const std::string& name, const std::string& filepath, const std::string& texture)
	{
		s_Fonts[name] = std::make_shared<Font>(filepath, texture);
	}
}