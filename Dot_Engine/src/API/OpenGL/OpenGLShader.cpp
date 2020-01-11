#include "stdafx.h"
#include "OpenGLShader.h"

#include <array>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

namespace Dot {
	
	std::unordered_map<std::string, std::shared_ptr<OpenGLUniformBuffer> > OpenGLShader::s_UBO;
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
		: m_Name(name),m_UniformsSize(0)
	{
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);
		Parse();	
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
	const uint32_t OpenGLShader::GetRendererID() const
	{
		return m_RendererID;
	}
	const Uniform* OpenGLShader::FindUniform(const std::string& name)
	{
		if (m_Uniforms.find(name) != m_Uniforms.end())
		{
			return &m_Uniforms[name];
		}
		return nullptr;
	}
	void OpenGLShader::LinkUniformBufferObject(const std::string& name, unsigned int bindIndex, unsigned int size)
	{
		unsigned int blockIndex = glGetUniformBlockIndex(m_RendererID, name.c_str());
		glUniformBlockBinding(m_RendererID, blockIndex, bindIndex);
	}
	void OpenGLShader::AddUniform(UniformDataType type, unsigned int size, unsigned int offset, const std::string& name)
	{
		int id = glGetUniformLocation(m_RendererID, name.c_str());
		if (id != -1)
		{
			m_Uniforms[name].size = size;
			m_Uniforms[name].offset = offset;
			m_Uniforms[name].type = type;
			m_Uniforms[name].ID = id;
		}
	}
	void OpenGLShader::SetUniform(const std::string& name, unsigned char* data)
	{
		auto uniform = m_Uniforms[name];
		switch (uniform.type)
		{
		case UniformDataType::FLOAT:
			UploadUniformFloat(name, *(float*)& data[uniform.offset]);
			break;
		case UniformDataType::FLOAT_VEC2:
			UploadUniformFloat2(name,*(glm::vec2*)&data[uniform.offset]);
			break;
		case UniformDataType::FLOAT_VEC3:
			UploadUniformFloat3(name,*(glm::vec3*)&data[uniform.offset]);
			break;
		case UniformDataType::FLOAT_VEC4:
			UploadUniformFloat4(name,*(glm::vec4*)&data[uniform.offset]);
			break;
		case UniformDataType::INT:
			UploadUniformInt(name,*(int*)&data[uniform.offset]);
			break;
		case UniformDataType::FLOAT_MAT4:
			UploadUniformMat4(name,*(glm::mat4*)&data[uniform.offset]);
			break;
		};
	}
	void OpenGLShader::UpdateUniformBufferObject(const std::string& name, const void* data, unsigned int size, int offset)
	{
		s_UBO[name]->Update(data, size, offset);
	}
	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		glUniform1i(m_Uniforms[name].ID, value);
	}
	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		glUniform1f(m_Uniforms[name].ID, value);
	}
	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		glUniform2f(m_Uniforms[name].ID, value.x, value.y);
	}
	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		glUniform3f(m_Uniforms[name].ID, value.x, value.y, value.z);
	}
	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		glUniform4f(m_Uniforms[name].ID, value.x, value.y, value.z, value.w);
	}
	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		glUniformMatrix3fv(m_Uniforms[name].ID, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(m_Uniforms[name].ID, 1, GL_FALSE, glm::value_ptr(matrix));
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
	void OpenGLShader::Parse()
	{
		GLint count;
		GLint size;
		GLenum type;
		const GLsizei bufSize = 50;
		GLchar name[bufSize];
		GLsizei length;
		GLint blockSize;
		GLint blockBind;
		// Parse uniforms
		glGetProgramiv(m_RendererID, GL_ACTIVE_UNIFORMS, &count);
		int offset = 0;
		for (int i = 0; i < count; i++)
		{
			int sizeUni = 0;
			glGetActiveUniform(m_RendererID, (GLuint)i, bufSize, &length, &size, &type, name);
			UniformDataType uniType = UniformDataType::NONE;
			switch (type)
			{
			case GL_FLOAT:	    uniType = UniformDataType::FLOAT;
				sizeUni = sizeof(float);
				break;
			case GL_FLOAT_VEC2: uniType = UniformDataType::FLOAT_VEC2;
				sizeUni = 2 * sizeof(float);
				break;
			case GL_FLOAT_VEC3: uniType = UniformDataType::FLOAT_VEC3;
				sizeUni = 3 * sizeof(float);
				break;
			case GL_FLOAT_VEC4: uniType = UniformDataType::FLOAT_VEC4;
				sizeUni = 4 * sizeof(float);
				break;
			case GL_INT:	    uniType = UniformDataType::INT;
				sizeUni = sizeof(int);
				break;
			case GL_INT_VEC2:	uniType = UniformDataType::INT_VEC2;
				sizeUni = 2 * sizeof(int);
				break;
			case GL_INT_VEC3:	uniType = UniformDataType::INT_VEC3;
				sizeUni = 3 * sizeof(int);
				break;
			case GL_INT_VEC4:	uniType = UniformDataType::INT_VEC4;
				sizeUni = 4 * sizeof(int);
				break;
			case GL_FLOAT_MAT4:	uniType = UniformDataType::FLOAT_MAT4;
				sizeUni = sizeof(glm::mat4);
				break;
			}
			
			if (size > 1)
			{
				for (size_t i = 0; i < size; i++)
				{
					std::string nameStr(name);
					std::size_t pos = nameStr.find("[");
					std::string subStr = nameStr.substr(0, pos + 1);
					std::string uniformName = subStr + std::to_string(i) + std::string("]");
					AddUniform(uniType, sizeUni, offset, uniformName);
					offset += sizeUni;
				}
			}
			else
			{	
				AddUniform(uniType, sizeUni, offset, name);
				offset += sizeUni;
			}
			
			m_UniformsSize += sizeUni;
		}
			
		//Parse buffer objects and link them with shader
		glGetProgramiv(m_RendererID, GL_ACTIVE_UNIFORM_BLOCKS, &count);
		for (int i = 0; i < count; i++)
		{		
			glGetActiveUniformBlockName(m_RendererID, (GLuint)i, bufSize, &size, name);
			glGetActiveUniformBlockiv(m_RendererID, (GLuint)i, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
			glGetActiveUniformBlockiv(m_RendererID, (GLuint)i, GL_UNIFORM_BLOCK_BINDING, &blockBind);
			
			if (s_UBO.find(name) == s_UBO.end())
			{
				s_UBO[name] = std::make_shared<OpenGLUniformBuffer>((void*)NULL, blockSize, blockBind);
				LinkUniformBufferObject(name, blockBind, blockSize);
			}
			else
			{
				LinkUniformBufferObject(name, blockBind, blockSize);
			}
			
		}

	}
}