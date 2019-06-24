#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "GuiTransform.h"



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

	void Update(GuiTransform& transform);
	void UpdateColor(glm::vec3& color);
	void UpdateTexOffset(float texOffset);

	void UpdateTransparency(float value);
	
	static std::string LoadShader(const std::string& filename);
	static GLuint CreateShader(const std::string& text, GLenum shaderType);

	

private:
	static const unsigned int NUM_SHADER = 2;


	enum
	{
		MVP_U,

		POS_U,

		SCALE_U,

		COLOR_U,

		TEX_OFFSET_U,

		TRANSPARENT_U,

		NUM_UNIFORMS
	};

	GLuint m_program;

	GLuint m_shaders[NUM_SHADER];
	GLuint m_uniforms[NUM_UNIFORMS];

};

