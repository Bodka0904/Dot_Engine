#include "stdafx.h"
#include "Shader.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

namespace Dot {

	Shader::Shader(const std::string& filename)
	{
		m_program = glCreateProgram();
	
		m_shaders.resize(2);
		m_shaders[0] = CreateShader(LoadShader(filename + ".vs"), GL_VERTEX_SHADER);
		m_shaders[1] = CreateShader(LoadShader(filename + ".fs"), GL_FRAGMENT_SHADER);
		
		glUseProgram(m_program);

	}

	Shader::~Shader()
	{
		glDeleteProgram(m_program);
		for (unsigned int i = 0; i < m_shaders.size(); i++)
		{
			glDetachShader(m_program, m_shaders[i]);
			glDeleteShader(m_shaders[i]);
		}	
	}


	void Shader::SetUniforms()
	{
		m_uniforms[VIEW_POS_U] = glGetUniformLocation(m_program, "viewPos");
		m_uniforms[LIGHT_STR_U] = glGetUniformLocation(m_program, "lightStrength");
		m_uniforms[SPEC_STR_U] = glGetUniformLocation(m_program, "specStrength");
		m_uniforms[LIGHT_COLOR_U] = glGetUniformLocation(m_program, "lightColor");
		m_uniforms[LIGHT_POS_U] = glGetUniformLocation(m_program, "lightPos");

	}

	void Shader::AddGeometryShader(const std::string& filename)
	{
		m_shaders.push_back(CreateShader(LoadShader(filename), GL_GEOMETRY_SHADER));
	}

	void Shader::SetLayout(const ShaderLayout& layout)
	{
		for (unsigned int i = 0; i < m_shaders.size(); i++)
		{
			glAttachShader(m_program, m_shaders[i]);
		}

		for (auto i : layout.m_attributes)
		{
			glBindAttribLocation(m_program, i.m_location, i.m_name.c_str());
		}
		
	}

	void Shader::LinkShader()
	{
		
	
		glUseProgram(m_program);
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
			for (int i = 0; i < m_shaders.size(); ++i)
			{
				glDeleteShader(m_shaders[i]);
			}

			LOG_ERR("Shader: Could not link shader %s", &infoLog[0])

		}

		glValidateProgram(m_program);

		
	}

	void Shader::Clean()
	{
		for (unsigned int i = 0; i < m_shaders.size(); i++)
		{
			glDetachShader(m_program, m_shaders[i]);
			glDeleteShader(m_shaders[i]);

		}
	}

	void Shader::Update() const
	{	
		glm::vec3 test = { 0.3,1,0.5 };
		glUniform1f(m_uniforms[LIGHT_STR_U], BasicLight.lightStrength);
		glUniform1f(m_uniforms[SPEC_STR_U], BasicLight.specStrength);
		glUniform3fv(m_uniforms[VIEW_POS_U], 1, &test[0]);
		glUniform3fv(m_uniforms[LIGHT_COLOR_U], 1, &BasicLight.lightColor[0]);
		glUniform3fv(m_uniforms[LIGHT_POS_U], 1, &BasicLight.lightPos[0]);

	}

	void Shader::Bind() const
	{

		glUseProgram(m_program);
	}

	void Shader::UnBind() const
	{
		glUseProgram(0);
	}

	void Shader::AddUniformBufferObject(const std::string& name, unsigned int bindIndex,unsigned int size)
	{
		m_UBO[name].reset(new UniformBuffer(NULL,size,bindIndex));
		m_UBO[name]->GetBlockIndex() = glGetUniformBlockIndex(m_program, name.c_str());

		glUniformBlockBinding(m_program, m_UBO[name]->GetBlockIndex(), m_UBO[name]->GetIndex());
	}

	void Shader::UpdateUniformBufferObject(const std::string& name, const void* data)
	{
		m_UBO[name]->Update(data);
	}

	void Shader::AddUniform(const std::string& name)
	{
		m_Uniforms[name] = glGetUniformLocation(m_program, name.c_str());
	}

	void Shader::UploadUniformMat4(const std::string& name , const glm::mat4& matrix)
	{
		glUniformMatrix4fv(m_Uniforms[name], 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::UploadUniformVec2(const std::string& name, const glm::vec2& vector)
	{
		glUniform2f(m_Uniforms[name],vector.x,vector.y);
	}

	void Shader::UploadFloat(const std::string& name, float value)
	{
		glUniform1f(m_Uniforms[name], value);
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
			LOG_ERR("Shader: Could not open file: %s", filename.c_str());
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

			LOG_ERR("Shader: Could not compile shader: %s", &errorLog[0]);

			glDeleteShader(shader);
		}
		return shader;
	}


}