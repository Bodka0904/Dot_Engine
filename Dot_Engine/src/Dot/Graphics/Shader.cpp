#include "Shader.h"



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
	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "texCoord");
	glBindAttribLocation(m_program, 2, "normal");

	glLinkProgram(m_program);
	//SetAttribs();
	//LinkShader();
	//SetUniforms();
}


void Shader::SetAttribs()
{
	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "texCoord");
	glBindAttribLocation(m_program, 2, "normal");
}

void Shader::SetUniforms()
{
	m_uniforms[TRANSFORM_U] = glGetUniformLocation(m_program, "transform");
	m_uniforms[LIGHT_STR_U] = glGetUniformLocation(m_program, "lightStrength");
	m_uniforms[LIGHT_COLOR_U] = glGetUniformLocation(m_program, "lightColor");
	m_uniforms[LIGHT_POS_U] = glGetUniformLocation(m_program, "lightPos");
	m_uniforms[LIGHT_DIR_U] = glGetUniformLocation(m_program, "lightDir");
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


		LOG_ERR("Shader: Could not link shader ",&infoLog[0])
	
	}
	glValidateProgram(m_program);
	glDetachShader(m_program, m_shaders[0]);
	glDetachShader(m_program, m_shaders[1]);
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
		//TO DO
	}
	return output;
}

GLuint Shader::CreateShader(const std::string & text, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
	{
		LOG_ERR("Shader: Could not create shader")
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

		LOG_ERR("Shader: Could not compile shader: %s",&errorLog[0]);
		
		glDeleteShader(shader);
	}	
	return shader;
}
