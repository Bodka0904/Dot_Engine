#include "stdafx.h"
#include "OpenGLShader.h"

#include <array>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

namespace Dot {
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
	OpenGLShader::OpenGLShader(const std::string& name, const std::string& filepath)
	{
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		m_Name = name;
	}
	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}
	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}
	void OpenGLShader::Compute(unsigned int groupX, unsigned int groupY, unsigned int groupZ) const
	{
		glUseProgram(m_RendererID);
		glDispatchCompute(groupX, groupY, groupZ);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}
	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}
	const std::string& OpenGLShader::GetName() const
	{
		return m_Name;
	}
	void OpenGLShader::AddUniformBufferObject(const std::string& name, unsigned int bindIndex, unsigned int size)
	{
		m_UBO[name].reset(new UniformBuffer(NULL, size, bindIndex));
		m_UBO[name]->GetBlockIndex() = glGetUniformBlockIndex(m_RendererID, name.c_str());
		glUniformBlockBinding(m_RendererID, m_UBO[name]->GetBlockIndex(), m_UBO[name]->GetIndex());
	}
	void OpenGLShader::AddUniform(const std::string& name)
	{
		m_Uniforms[name] = glGetUniformLocation(m_RendererID, name.c_str());
	}
	void OpenGLShader::UpdateUniformBufferObject(const std::string& name, const void* data, unsigned int size)
	{
		m_UBO[name]->Update(data, size);
	}
	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		glUniform1i(m_Uniforms[name], value);
	}
	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		glUniform1f(m_Uniforms[name], value);
	}
	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		glUniform2f(m_Uniforms[name], value.x, value.y);
	}
	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		glUniform3f(m_Uniforms[name], value.x, value.y, value.z);
	}
	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		glUniform4f(m_Uniforms[name], value.x, value.y, value.z, value.w);
	}
	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		glUniformMatrix3fv(m_Uniforms[name], 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(m_Uniforms[name], 1, GL_FALSE, glm::value_ptr(matrix));
	}
	std::string OpenGLShader::ReadFile(const std::string& filepath)
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
	std::unordered_map<unsigned int, std::string> OpenGLShader::PreProcess(const std::string& source)
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
	void OpenGLShader::Compile(const std::unordered_map<unsigned int, std::string>& shaderSources)
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
}