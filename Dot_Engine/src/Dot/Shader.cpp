#include "Shader.h"



Shader::Shader(const std::string& filename)
{

	m_program = glCreateProgram();
	m_shaders[0] = CreateShader(LoadShader(filename + ".vs"), GL_VERTEX_SHADER);
	m_shaders[1] = CreateShader(LoadShader(filename + ".fs"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADER; i++)
	{
		glAttachShader(m_program, m_shaders[i]);
	}
	LOG_INFO("Shader: Constructed");


}



Shader::~Shader()
{
	glDeleteProgram(m_program);
	for (unsigned int i = 0; i < NUM_SHADER; i++)
	{
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);

	}
	LOG_INFO("Shader: Destroyed");
}


void Shader::SetAttrib(unsigned int location, const GLchar* name)
{
	glBindAttribLocation(m_program, location, name);
}

void Shader::SetUniform(unsigned int uniform, const GLchar * name)
{
	m_uniforms[uniform] = glGetUniformLocation(m_program, name);
}

void Shader::LinkShader()
{
	glLinkProgram(m_program);
	glValidateProgram(m_program);

}

void Shader::Update(const Transform & transform, const Camera& camera)
{

	glm::mat4 model = camera.GetViewProjection() * transform.GetModel();
	glUniformMatrix4fv(m_uniforms[TRANSFORM_U], 1, GL_FALSE, &model[0][0]);
	glUniform1f(m_uniforms[LIGHT_STR_U], BasicLight.lightStrength);
	glUniform3fv(m_uniforms[LIGHT_COLOR_U], sizeof(glm::vec3), &BasicLight.lightColor[0]);
	glUniform3fv(m_uniforms[LIGHT_POS_U], sizeof(glm::vec3), &BasicLight.lightPos[0]);
	glUniform3fv(m_uniforms[LIGHT_DIR_U], sizeof(glm::vec3), &BasicLight.lightDir[0]);

}

void Shader::Bind()
{
	LOG_INFO("Shader: BIND");
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
		LOG_ERR("Shader: unable to load shader file");
	}
	return output;
}

GLuint Shader::CreateShader(const std::string & text, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
	{
		LOG_ERR("Shader: unable to create shader");
	}
	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLength[1];

	shaderSourceStrings[0] = text.c_str();    //convert regular string to C-string;
	shaderSourceStringLength[0] = text.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLength);
	glCompileShader(shader);

	return shader;
}
