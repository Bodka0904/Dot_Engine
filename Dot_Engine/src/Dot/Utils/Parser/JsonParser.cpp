#include "stdafx.h"
#include "JsonParser.h"

namespace Dot {
	Json JsonParser::ParseJson(const std::string& filepath, const std::vector<std::string>& keywords)
	{
		Json json;
		
		std::ifstream file;
		file.open(filepath);
		
		std::string line;
		std::string tmp;
		while (std::getline(file, line))
		{
			for (int i = 0; i < keywords.size(); ++i)
			{
				if (std::strstr(line.c_str(), keywords[i].c_str()))
				{
					JsonObject tmpObject;
					while (std::getline(file, line))
					{
						
						if (!std::strstr(line.c_str(), "[") && !std::strstr(line.c_str(), "{") && 
							!std::strstr(line.c_str(), "]") && !std::strstr(line.c_str(), "}"))
						{			
							
							std::vector<std::string> output = DelimiterString(line, ":");
							
							for (int i = 0; i < output.size(); i += 2)
							{
								JsonAttribute tmpAttr;

								EraseFromString(output[i], '"');
								EraseFromString(output[i + 1], '"');
								EraseFromString(output[i], ',');
								EraseFromString(output[i + 1], ',');
								EraseFromString(output[i], ' ');
								EraseFromString(output[i + 1], ' ');

								tmpAttr.m_Value.first = output[i];
								tmpAttr.m_Value.second = output[i + 1];

								tmpObject.m_Attribute.push_back(tmpAttr);

							}	
							
						}
						if (std::strstr(line.c_str(), "}"))
						{
							json.m_Object[keywords[i]].push_back(tmpObject);
							tmpObject.m_Attribute.clear();
						}
						if (std::strstr(line.c_str(), "]"))
						{
							// Look for new keyword
							break;
						}
					}
					
				}
			}
		}
		
		return json;
	}
	std::vector<std::string> JsonParser::DelimiterString(const std::string& str, const std::string& delimiter)
	{	
		std::vector<std::string> strings;
		std::string::size_type pos = 0;
		std::string::size_type prev = 0;
		while ((pos = str.find(delimiter, prev)) != std::string::npos)
		{
			strings.push_back(str.substr(prev, pos - prev));
			prev = pos + 1;
		}

		// To get the last substring (or only, if delimiter is not found)
		strings.push_back(str.substr(prev));
		return strings;	
	}
	void JsonParser::EraseFromString(std::string& str, const char character)
	{
		std::string::size_type pos = 0;
		std::string::size_type prev = 0;
		str.erase(std::remove(str.begin(), str.end(), character), str.end());
	}
}