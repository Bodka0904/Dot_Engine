#pragma once
#include "stdafx.h"
#include "Font.h"
#include <fstream>

#include "Dot/Debug/Log.h"

namespace Dot {
	std::unordered_map<std::string, Ref<Font> > Font::s_Fonts;

	Font::Font(const std::string& filepath, const std::string& texture)
	{
		m_Texture = Texture2D::Create(texture, false, false);
		std::string line;
		std::ifstream FILE(filepath);
	
		if (FILE.is_open())
		{	
			int lineCount = 0;
			int correct = 0;
			while (std::getline(FILE, line).good())
			{
				if (lineCount == 1)
				{
					correct = sscanf(line.c_str(), "common lineHeight=%lf base=%d scaleW=%d scaleH=%d",
						&m_MetaData.lineHeight,&m_MetaData.base, &m_MetaData.scaleW, &m_MetaData.scaleH);
					if (correct != 4)
						break;
				}
				else if (lineCount == 3)
				{
					correct = sscanf(line.c_str(), "chars count=%d", &m_MetaData.count);
					if (correct != 1)
						break;
				}
				else if (lineCount > 3)
				{
					Character tmp;
					correct = sscanf(line.c_str(),
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
				lineCount++;
			}
			FILE.close();
		}
		else
		{
			LOG_ERR("Could not open the file! %s", filepath.c_str());
		}
	
		if (m_Characters.size() != m_MetaData.count)
		{
			LOG_ERR("Failed to read all characters data! %s", filepath.c_str());
		}
	}
	void Font::AddFont(const std::string& name, const std::string& filepath, const std::string& texture)
	{
		s_Fonts[name] = std::make_shared<Font>(filepath, texture);
	}
}