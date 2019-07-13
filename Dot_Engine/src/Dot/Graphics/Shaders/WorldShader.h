#pragma once
#include "Shader.h"

namespace Dot {
	class WorldShader : public Shader
	{
	public:
		WorldShader(const std::string& filename);
		virtual ~WorldShader();


	private:
		std::map<std::string, std::unique_ptr<UniformBuffer> > m_UBO;
		std::map<std::string, unsigned int> m_Uniforms;

	private:
		static const unsigned int NUM_SHADER = 2;
		unsigned int m_program;
		unsigned int m_shaders[NUM_SHADER];
	};

}