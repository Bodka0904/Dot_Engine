#pragma once
#include <string>
#include <iostream>
#include <conio.h>
#include "Dot/Log.h"
#include <vector>
#include <fstream>
#include "Transform.h"
#include "Camera.h"
#include <GL/glew.h>
#include <GLFW\glfw3.h>


struct Light
{
	float	 lightStrength;
	glm::vec3 lightColor;
	glm::vec3 lightPos;
	glm::vec3 lightDir;

	Light(float lightStr = 0.8, glm::vec3 lightC = glm::vec3(0.5, 0.5, 0.5),
		glm::vec3 lightP = glm::vec3(0, 1, 0), glm::vec3 lightD = glm::vec3(0,0,0))
		:
		lightStrength(lightStr),lightColor(lightC),lightPos(lightP),lightDir(lightD)
	{}

};

class Shader
{
public:
	Shader();
	
	void Init(const std::string& filename);
	
	virtual void SetAttribs();
	virtual void SetUniforms();
	virtual void Update(const Transform& transform, const Camera& camera);
	
	void LinkShader();
	void Bind();
	void UnBind();



	static std::string LoadShader(const std::string& filename);
	static GLuint CreateShader(const std::string& text, GLenum shaderType);

	virtual ~Shader();

private:
	static const unsigned int NUM_SHADER = 2;
	
	enum
	{
		TRANSFORM_U,

		LIGHT_STR_U,

		LIGHT_COLOR_U,

		LIGHT_POS_U,

		LIGHT_DIR_U,

		NUM_UNIFORMS
	};

	Light BasicLight;

	GLuint m_program;
	GLuint m_shaders[NUM_SHADER];
	GLuint m_uniforms[NUM_UNIFORMS];

	

};
