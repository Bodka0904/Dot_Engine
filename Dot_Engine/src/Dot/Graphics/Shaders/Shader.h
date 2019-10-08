#pragma once
#include <conio.h>
#include <glm/glm.hpp>

#include "Dot/Debug/Log.h"
#include "Uniform/UniformBuffer.h"


namespace Dot {

	class Shader
	{
	public:
		Shader(const std::string& name,const std::string & filepath);
		~Shader();

		void Bind() const;
		void Compute(unsigned int groupX, unsigned int groupY = 1, unsigned int groupZ = 1) const;
		void Unbind() const;

		const std::string& GetName() const { return m_Name; }

		void AddUniformBufferObject(const std::string& name, unsigned int bindIndex, unsigned int size);
		void AddUniform(const std::string& name);
		
		void UpdateUniformBufferObject(const std::string& name, const void* data, unsigned int size);
		void UploadUniformInt(const std::string& name, int value);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
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


	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};

}