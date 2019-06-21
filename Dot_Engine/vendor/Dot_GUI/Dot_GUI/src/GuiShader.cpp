#include "stdafx.h"
#include "GuiShader.h"


GuiShader::GuiShader()
{
}


GuiShader::~GuiShader()
{
	glDeleteProgram(m_program);
	for (unsigned int i = 0; i < NUM_SHADER; i++)
	{
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);

	}
}

void GuiShader::Init(const std::string & filename)
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
}

void GuiShader::SetAttribs()
{
	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "color");
	glBindAttribLocation(m_program, 2, "texCoords");
}

void GuiShader::SetUniforms()
{
	m_uniforms[MVP_U] = glGetUniformLocation(m_program, "mvp_u");
	m_uniforms[POS_U] = glGetUniformLocation(m_program, "pos_u");
	m_uniforms[SCALE_U] = glGetUniformLocation(m_program, "scale_u");
	m_uniforms[COLOR_U] = glGetUniformLocation(m_program, "color_u");
}


void GuiShader::LinkShader()
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


	

	}
	glValidateProgram(m_program);
	glDetachShader(m_program, m_shaders[0]);
	glDetachShader(m_program, m_shaders[1]);
}

void GuiShader::Bind()
{
	glUseProgram(m_program);
}

void GuiShader::UnBind()
{
	glUseProgram(0);
}



void GuiShader::Update(GuiTransform & transform)
{	
	glUniformMatrix4fv(m_uniforms[MVP_U], 1, GL_FALSE, &transform.GetOrtho()[0][0]);
	glUniform2fv(m_uniforms[POS_U], 1, &transform.GetPos()[0]);
	glUniform2fv(m_uniforms[SCALE_U], 1, &transform.GetScale()[0]);
}



void GuiShader::UpdateColor(glm::vec3 & color)
{
	glUniform3fv(m_uniforms[COLOR_U], 1, &color[0]);
}

std::string GuiShader::LoadShader(const std::string & filename)
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
		
	}
	return output;
}

GLuint GuiShader::CreateShader(const std::string & text, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
	{
		
	}
	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLength[1];

	shaderSourceStrings[0] = text.c_str();
	shaderSourceStringLength[0] = text.length();

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

	

		glDeleteShader(shader);
	}
	return shader;
}
