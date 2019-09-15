#include "stdafx.h"
#include "Shader.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <array>

namespace Dot {

//	Shader::Shader(const std::string& vsSrc, const std::string& fsSrc)
//	{
//		m_program = glCreateProgram();
//	
//		m_shaders.resize(2);
//		m_shaders[0] = CreateShader(LoadShader(vsSrc), GL_VERTEX_SHADER);
//		m_shaders[1] = CreateShader(LoadShader(fsSrc), GL_FRAGMENT_SHADER);
//		
//		glUseProgram(m_program);
//
//	}
//
//	Shader::~Shader()
//	{
//		glDeleteProgram(m_program);
//		for (unsigned int i = 0; i < m_shaders.size(); i++)
//		{
//			glDetachShader(m_program, m_shaders[i]);
//			glDeleteShader(m_shaders[i]);
//		}	
//	}
//
//
//	void Shader::SetUniforms()
//	{
//		m_uniforms[VIEW_POS_U] = glGetUniformLocation(m_program, "viewPos");
//		m_uniforms[LIGHT_STR_U] = glGetUniformLocation(m_program, "lightStrength");
//		m_uniforms[SPEC_STR_U] = glGetUniformLocation(m_program, "specStrength");
//		m_uniforms[LIGHT_COLOR_U] = glGetUniformLocation(m_program, "lightColor");
//		m_uniforms[LIGHT_POS_U] = glGetUniformLocation(m_program, "lightPos");
//
//	}
//
//	void Shader::AddGeometryShader(const std::string& filename)
//	{
//		m_shaders.push_back(CreateShader(LoadShader(filename), GL_GEOMETRY_SHADER));
//	}
//
//	void Shader::SetLayout(const ShaderLayout& layout)
//	{
//		for (unsigned int i = 0; i < m_shaders.size(); i++)
//		{
//			glAttachShader(m_program, m_shaders[i]);
//		}
//
//		for (auto i : layout.m_attributes)
//		{
//			glBindAttribLocation(m_program, i.m_location, i.m_name.c_str());
//		}
//		
//	}
//
//	void Shader::LinkShader()
//	{
//		
//	
//		glUseProgram(m_program);
//		glLinkProgram(m_program);
//
//		GLint isLinked = 0;
//		glGetProgramiv(m_program, GL_LINK_STATUS, (int *)&isLinked);
//		
//		if (isLinked == GL_FALSE)
//		{
//			GLint maxLength = 0;
//			glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &maxLength);
//
//			// The maxLength includes the NULL character
//			std::vector<GLchar> infoLog(maxLength);
//			glGetProgramInfoLog(m_program, maxLength, &maxLength, &infoLog[0]);
//
//
//			glDeleteProgram(m_program);
//			for (int i = 0; i < m_shaders.size(); ++i)
//			{
//				glDeleteShader(m_shaders[i]);
//			}
//
//			LOG_ERR("Shader: Could not link shader %s", &infoLog[0]);
//
//		}
//
//		glValidateProgram(m_program);
//
//
//		for (unsigned int i = 0; i < m_shaders.size(); i++)
//		{
//			glDetachShader(m_program, m_shaders[i]);
//			glDeleteShader(m_shaders[i]);
//
//		}
//	}
//
//
//	void Shader::Update() const
//	{	
//		glm::vec3 test = { 0.0f,-1.0f,0.0f };
//		glUniform1f(m_uniforms[LIGHT_STR_U], BasicLight.lightStrength);
//		glUniform1f(m_uniforms[SPEC_STR_U], BasicLight.specStrength);
//		glUniform3fv(m_uniforms[VIEW_POS_U], 1, &test[0]);
//		glUniform3fv(m_uniforms[LIGHT_COLOR_U], 1, &BasicLight.lightColor[0]);
//		glUniform3fv(m_uniforms[LIGHT_POS_U], 1, &BasicLight.lightPos[0]);
//
//	}
//
//	void Shader::Bind() const
//	{
//
//		glUseProgram(m_program);
//	}
//
//	void Shader::UnBind() const
//	{
//		glUseProgram(0);
//	}
//
//	void Shader::AddUniformBufferObject(const std::string& name, unsigned int bindIndex,unsigned int size)
//	{
//		m_UBO[name].reset(new UniformBuffer(NULL,size,bindIndex));
//		m_UBO[name]->GetBlockIndex() = glGetUniformBlockIndex(m_program, name.c_str());
//
//		glUniformBlockBinding(m_program, m_UBO[name]->GetBlockIndex(), m_UBO[name]->GetIndex());
//	}
//
//	void Shader::UpdateUniformBufferObject(const std::string& name, const void* data,unsigned int size)
//	{
//		m_UBO[name]->Update(data,size);
//	}
//
//	void Shader::AddUniform(const std::string& name)
//	{
//		m_Uniforms[name] = glGetUniformLocation(m_program, name.c_str());
//	}
//
//
//	void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
//	{
//		glUniformMatrix4fv(m_Uniforms[name], 1, GL_FALSE, glm::value_ptr(matrix));
//	}
//
//	void Shader::UploadUniformVec2(const std::string& name, const glm::vec2& vector)
//	{
//		glUniform2f(m_Uniforms[name],vector.x,vector.y);
//	}
//
//	void Shader::UploadFloat(const std::string& name, float value)
//	{
//		glUniform1f(m_Uniforms[name], value);
//	}
//
//	void Shader::UploadInt(const std::string& name, int value)
//	{
//		glUniform1i(m_Uniforms[name], value);
//	}
//
//
//
//	std::string Shader::LoadShader(const std::string & filename)
//	{
//		std::ifstream file;
//		file.open((filename).c_str());
//
//		std::string output;
//		std::string line;
//
//		if (file.is_open())
//		{
//			while (file.good())
//			{
//				getline(file, line);
//				output.append(line + "\n");
//			}
//		}
//
//		else
//		{
//			LOG_ERR("Shader: Could not open file: %s", filename.c_str());
//		}
//		return output;
//	}
//
//	unsigned int Shader::CreateShader(const std::string & text, unsigned int shaderType)
//	{
//		GLuint shader = glCreateShader(shaderType);
//
//		if (shader == 0)
//		{
//			LOG_ERR("Shader: Could not create shader");
//		}
//		const GLchar* shaderSourceStrings[1];
//		GLint shaderSourceStringLength[1];
//
//		shaderSourceStrings[0] = text.c_str();
//		shaderSourceStringLength[0] = (size_t)text.length();
//
//		glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLength);
//
//		glCompileShader(shader);
//		GLint isCompiled = 0;
//		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
//		if (isCompiled == GL_FALSE)
//		{
//			GLint maxLength = 0;
//			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
//
//			std::vector<GLchar> errorLog(maxLength);
//			glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
//
//			LOG_ERR("Shader: Could not compile shader: %s", &errorLog[0]);
//
//			glDeleteShader(shader);
//		}
//		return shader;
//	}
//
//
//	ComputeShader::ComputeShader(const std::string& src)
//	{
//		m_program = glCreateProgram();
//
//		m_shader = Shader::CreateShader(Shader::LoadShader(src), GL_COMPUTE_SHADER);
//	
//		glAttachShader(m_program, m_shader);
//		LinkShader();
//	}
//
//	ComputeShader::~ComputeShader()
//	{
//	}
//
//	void ComputeShader::LinkShader()
//	{
//
//		glUseProgram(m_program);
//		glLinkProgram(m_program);
//
//		GLint isLinked = 0;
//		glGetProgramiv(m_program, GL_LINK_STATUS, (int*)& isLinked);
//		
//		if (isLinked == GL_FALSE)
//		{
//			GLint maxLength = 0;
//			glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &maxLength);
//
//			// The maxLength includes the NULL character
//			std::vector<GLchar> infoLog(maxLength);
//			glGetProgramInfoLog(m_program, maxLength, &maxLength, &infoLog[0]);
//
//
//			glDeleteProgram(m_program);
//			glDeleteShader(m_shader);
//			
//			LOG_ERR("Shader: Could not link shader %s", &infoLog[0]);
//
//		}
//	
//		glValidateProgram(m_program);
//
//		glDetachShader(m_program, m_shader);
//		glDeleteShader(m_shader);
//
//		
//	}
//
//	void ComputeShader::Compute(unsigned int groupX, unsigned int groupY, unsigned int groupZ)
//	{
//		glUseProgram(m_program);
//		glDispatchCompute(groupX, groupY, groupZ);
//		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
//	}
//
//	void ComputeShader::AddUniform(const std::string& name)
//	{
//		m_Uniforms[name] = glGetUniformLocation(m_program, name.c_str());
//	}
//
//	void ComputeShader::UploadUniformMat4(const std::string& name, const float* data, unsigned int count)
//	{
//	}
//
//	void ComputeShader::UploadUniformVec2(const std::string& name, const glm::vec2& vector)
//	{
//	}
//
//	void ComputeShader::UploadFloat(const std::string& name, float value)
//	{
//		glUniform1f(m_Uniforms[name], value);
//	}
//
//	void ComputeShader::UploadInt(const std::string& name, int value)
//	{
//	}
//

static GLenum ShaderTypeFromString(const std::string& type)
{
	if (type == "vertex")
		return GL_VERTEX_SHADER;
	if (type == "fragment" || type == "pixel")
		return GL_FRAGMENT_SHADER;
	if (type == "geometry")
		return GL_GEOMETRY_SHADER;
	if (type == "compute")
		return GL_COMPUTE_SHADER;

	D_ASSERT(false, "Unknown shader type!");
	return 0;
}

Shader::Shader(const std::string& name, const std::string& filepath)
{
	std::string source = ReadFile(filepath);
	auto shaderSources = PreProcess(source);
	Compile(shaderSources);


	m_Name = name;
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Compute(unsigned int groupX, unsigned int groupY, unsigned int groupZ) const
{
	glUseProgram(m_RendererID);
	glDispatchCompute(groupX, groupY, groupZ);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::AddUniformBufferObject(const std::string& name, unsigned int bindIndex, unsigned int size)
{
	m_UBO[name].reset(new UniformBuffer(NULL, size, bindIndex));
	m_UBO[name]->GetBlockIndex() = glGetUniformBlockIndex(m_RendererID, name.c_str());
	glUniformBlockBinding(m_RendererID, m_UBO[name]->GetBlockIndex(), m_UBO[name]->GetIndex());
}

void Shader::AddUniform(const std::string& name)
{
	m_Uniforms[name] = glGetUniformLocation(m_RendererID, name.c_str());
}

void Shader::UpdateUniformBufferObject(const std::string& name, const void* data, unsigned int size)
{
	m_UBO[name]->Update(data, size);
}

void Shader::UploadUniformInt(const std::string& name, int value)
{
	glUniform1i(m_Uniforms[name], value);
}

void Shader::UploadUniformFloat(const std::string& name, float value)
{
	glUniform1f(m_Uniforms[name], value);
}

void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
{
	glUniform2f(m_Uniforms[name], value.x, value.y);
}

void Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
{
	glUniform3f(m_Uniforms[name], value.x, value.y, value.z);
}

void Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
{
	glUniform4f(m_Uniforms[name], value.x, value.y, value.z, value.w);
}

void Shader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
{
	glUniformMatrix3fv(m_Uniforms[name], 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(m_Uniforms[name], 1, GL_FALSE, glm::value_ptr(matrix));
}

std::string Shader::ReadFile(const std::string& filepath)
{
	std::string result;
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		result.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();
		;
	}
	else
	{
		LOG_ERR("Could not open file %s ", filepath);
	}

	return result;
}

std::unordered_map<unsigned int, std::string> Shader::PreProcess(const std::string& source)
{
	std::unordered_map<GLenum, std::string> shaderSources;

	const char* typeToken = "#type";
	size_t typeTokenLength = strlen(typeToken);
	size_t pos = source.find(typeToken, 0);
	while (pos != std::string::npos)
	{
		size_t eol = source.find_first_of("\r\n", pos);
		D_ASSERT(eol != std::string::npos, "Syntax error");
		size_t begin = pos + typeTokenLength + 1;
		std::string type = source.substr(begin, eol - begin);
		D_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

		size_t nextLinePos = source.find_first_not_of("\r\n", eol);
		pos = source.find(typeToken, nextLinePos);
		shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
	}

	return shaderSources;
}

void Shader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
{
	GLuint program = glCreateProgram();
	D_ASSERT(shaderSources.size() <= 3, "We only support 3 shaders for now");
	std::array<GLenum, 3> glShaderIDs;

	int glShaderIDIndex = 0;
	for (auto& kv : shaderSources)
	{
		GLenum type = kv.first;
		const std::string& source = kv.second;

		GLuint shader = glCreateShader(type);

		const GLchar* sourceCStr = source.c_str();
		glShaderSource(shader, 1, &sourceCStr, 0);

		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(shader);

			LOG_ERR("%s", infoLog.data());
			D_ASSERT(false, "Shader compilation failure!");
			break;
		}

		glAttachShader(program, shader);
		glShaderIDs[glShaderIDIndex++] = shader;
	}

	m_RendererID = program;

	// Link our program
	glLinkProgram(program);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)& isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(program);

		for (auto id : glShaderIDs)
			glDeleteShader(id);

		LOG_ERR("%s", infoLog.data());
		D_ASSERT(false, "Shader link failure!");
		return;
	}

	for (auto id : glShaderIDs)
		glDetachShader(program, id);
}

void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
{
}

void ShaderLibrary::Add(const Ref<Shader>& shader)
{
}

Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
{
	return Ref<Shader>();
}

Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
{
	return Ref<Shader>();
}

Ref<Shader> ShaderLibrary::Get(const std::string& name)
{
	return Ref<Shader>();
}

bool ShaderLibrary::Exists(const std::string& name) const
{
	return false;
}

}
