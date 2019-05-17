#pragma once
#include <string>
#include <iostream>
#include <conio.h>
#include <fstream>
#include "Transform.h"
#include "Camera.h"
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include "Log.h"


class Shader
{
public:
	Shader(const std::string& filename,unsigned int NUM_UNIFORMS);

	void SetAttrib(unsigned int location, const GLchar* name);
	void SetUniform(unsigned int uniform, const GLchar* name);
	void LinkShader();
	void Update(const Transform& transform, const Camera& camera);
	void Bind();
	void UnBind();



	static std::string LoadShader(const std::string& filename);
	static GLuint CreateShader(const std::string& text, GLenum shaderType);

	virtual ~Shader();

private:
	static const unsigned int NUM_SHADER = 2;
	unsigned int NUM_UNIFORMS;

	GLuint m_program;
	GLuint m_shaders[NUM_SHADER];
	GLuint m_uniforms[NUM_UNIFORMS];



};
