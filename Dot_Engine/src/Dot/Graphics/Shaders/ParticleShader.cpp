#include "stdafx.h"
#include "ParticleShader.h"


ParticleShader::ParticleShader()
{
}


ParticleShader::~ParticleShader()
{
	glDeleteProgram(m_program);
	for (unsigned int i = 0; i < NUM_SHADER; i++)
	{
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);

	}
}

void ParticleShader::Init(const std::string & filename)
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

void ParticleShader::SetAttribs()
{
	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "color");
	glBindAttribLocation(m_program, 2, "offset");
}

void ParticleShader::SetUniforms()
{
	m_uniforms[TRANSFORM_U] = glGetUniformLocation(m_program, "transform");
}

void ParticleShader::Update(const Transform & transform, Camera & camera)
{
	glm::mat4 model = camera.GetViewProjection() * transform.GetModel();
	glUniformMatrix4fv(m_uniforms[TRANSFORM_U], 1, GL_FALSE, &model[0][0]);
}

void ParticleShader::LinkShader()
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


		LOG_ERR("Shader: Could not link shader ", &infoLog[0])

	}
	glValidateProgram(m_program);
	glDetachShader(m_program, m_shaders[0]);
	glDetachShader(m_program, m_shaders[1]);
}

void ParticleShader::Bind()
{
	glUseProgram(m_program);
}

void ParticleShader::UnBind()
{
	glUseProgram(0);
}
