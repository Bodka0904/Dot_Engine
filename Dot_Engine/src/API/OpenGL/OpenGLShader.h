#pragma once
#include "Dot/Renderer/Shaders/Shader.h"
#include "Dot/Renderer/Shaders/Uniform/UniformBuffer.h"

namespace Dot {
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& filepath);
		virtual ~OpenGLShader();
		virtual void Bind() const;
		virtual void Compute(unsigned int groupX, unsigned int groupY = 1, unsigned int groupZ = 1) const;
		virtual void Unbind() const;

		virtual const std::string& GetName() const;

		virtual void AddUniformBufferObject(const std::string& name, unsigned int bindIndex, unsigned int size);
		virtual void AddUniform(const std::string& name);

		virtual void UpdateUniformBufferObject(const std::string& name, const void* data, unsigned int size);
		virtual void UploadUniformInt(const std::string& name, int value);

		virtual void UploadUniformFloat(const std::string& name, float value);
		virtual void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		virtual void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);


	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<unsigned int, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<unsigned int, std::string>& shaderSources);

	private:
		uint32_t m_RendererID;
		std::string m_Name;

		std::unordered_map<std::string, std::unique_ptr<UniformBuffer> > m_UBO;
		std::unordered_map<std::string, unsigned int> m_Uniforms;
	};
}