#pragma once
#include "Dot/Graphics/Shaders/Shader.h"

namespace Dot {

	class Font
	{
	public:
		Font();
		~Font();

		static void AddFont(const std::string& name, const std::string& imagepath);
		static void BindFont(const std::string& name);

	private:
		static unsigned int LoadDDSFont(const char* imagepath);
		static unsigned int LoadBMPFont(const char* imagepath);

	private:
		static std::map<std::string,unsigned int> m_FontTexture;
	};
}