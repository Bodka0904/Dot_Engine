#include "stdafx.h"
#include "Shader.h"
#include <GL/glew.h>

Shader::Shader()
{
	
}



Shader::~Shader()
{
	glDeleteProgram(m_program);
	for (unsigned int i = 0; i < NUM_SHADER; i++)
	{
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);

	}
	
	
}

void Shader::Init(const std::string &filename)
{
	m_program = glCreateProgram();
	m_shaders[0] = CreateShader(LoadShader(filename + ".vs"), GL_VERTEX_SHADER);
	m_shaders[1] = CreateShader(LoadShader(filename + ".fs"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADER; i++)
	{
		glAttachShader(m_program, m_shaders[i]);
	}
	
	
	
	SetAttribs();
	LinkShader();

	glUseProgram(m_program);
	SetUniforms();

	for (unsigned int i = 0; i < NUM_SHADER; i++)
	{
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);

	}
}


void Shader::SetAttribs()
{
	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "normal");
	glBindAttribLocation(m_program, 2, "texCoord");
}

void Shader::SetUniforms()
{
	m_uniforms[TRANSFORM_U] = glGetUniformLocation(m_program, "transform");
	m_uniforms[VIEW_POS_U] = glGetUniformLocation(m_program, "viewPos");
	m_uniforms[LIGHT_STR_U] = glGetUniformLocation(m_program, "lightStrength");
	m_uniforms[SPEC_STR_U] = glGetUniformLocation(m_program, "specStrength");
	m_uniforms[LIGHT_COLOR_U] = glGetUniformLocation(m_program, "lightColor");
	m_uniforms[LIGHT_POS_U] = glGetUniformLocation(m_program, "lightPos");
	
}

void Shader::LinkShader()
{

	glLinkProgram(m_program);
	GLint isLinked = 0;
	glGetProgramiv(m_program, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked = GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_program, maxLength, &maxLength, &infoLog[0]);

		
		glDeleteProgram(m_program);
	
		glDeleteShader(m_shaders[0]);
		glDeleteShader(m_shaders[1]);


		LOG_ERR("Shader: Could not link shader %s",&infoLog[0])
	
	}
	glValidateProgram(m_program);
	glDetachShader(m_program, m_shaders[0]);
	glDetachShader(m_program, m_shaders[1]);
}

void Shader::Update(const Transform & transform, Camera& camera)
{

	glm::mat4 model = camera.GetViewProjection() * transform.GetModel();
	glUniformMatrix4fv(m_uniforms[TRANSFORM_U], 1, GL_FALSE, &model[0][0]);
	glUniform1f(m_uniforms[LIGHT_STR_U], BasicLight.lightStrength);
	glUniform1f(m_uniforms[SPEC_STR_U], BasicLight.specStrength);
	glUniform3fv(m_uniforms[VIEW_POS_U], 1, &camera.GetPos()[0]);
	glUniform3fv(m_uniforms[LIGHT_COLOR_U], 1, &BasicLight.lightColor[0]);
	glUniform3fv(m_uniforms[LIGHT_POS_U], 1, &BasicLight.lightPos[0]);
	
}

void Shader::Bind()
{
	
	glUseProgram(m_program);
}

void Shader::UnBind()
{
	glUseProgram(0);
}



std::string Shader::LoadShader(const std::string & filename)
{
	std::ifstream file;
	file.open((filename).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	
	else
	{
		LOG_ERR("Shader: Could not open file: %s",filename.c_str());
	}
	return output;
}

unsigned int Shader::CreateShader(const std::string & text, unsigned int shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
	{
		LOG_ERR("Shader: Could not create shader")
	}
	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLength[1];

	shaderSourceStrings[0] = text.c_str();   
	shaderSourceStringLength[0] = (size_t)text.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLength);

	glCompileShader(shader);
	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		LOG_ERR("Shader: Could not compile shader: %s",&errorLog[0]);
		
		glDeleteShader(shader);
	}	
	return shader;
}
