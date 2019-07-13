#pragma once
#include <string>
#include <glm/glm.hpp>
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
	void UpdateTexOffset(glm::vec2 texOffset);

	void UpdateTransparency(float value);
	
	static std::string LoadShader(const std::string& filename);
	static unsigned int CreateShader(const std::string& text, unsigned int shaderType);

	

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

	unsigned int m_program;

	unsigned int m_shaders[NUM_SHADER];
	unsigned int m_uniforms[NUM_UNIFORMS];

};

