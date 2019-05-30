#pragma once
#include "stdafx.h"
#include "Dot/Log.h"
#include <glm/glm.hpp>
#include <GL/glew.h>

class GuiShader
{
public:
	GuiShader();
	~GuiShader();

	void Init(const std::string& filename);

	void SetAttribs();
	void SetUniforms();

	void LinkShader();
	void Bind();
	void UnBind();

	void Update(glm::vec2& translate);


	static std::string LoadShader(const std::string& filename);
	static GLuint CreateShader(const std::string& text, GLenum shaderType);

	

private:
	static const unsigned int NUM_SHADER = 2;

	GLuint m_program;
	GLuint m_shaders[NUM_SHADER];
	GLuint m_uniforms[1];

};

