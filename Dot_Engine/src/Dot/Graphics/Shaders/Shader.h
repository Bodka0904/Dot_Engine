#pragma once
#include <conio.h>
#include <glm/glm.hpp>

#include "Dot/Debug/Log.h"
#include "Uniform/UniformBuffer.h"


namespace Dot {

	struct Light
	{
		float	 lightStrength;
		float	 specStrength;
		glm::vec3 lightColor;
		glm::vec3 lightPos;


		Light(float lightStr = 0.5, float specStr = 2, glm::vec3 lightC = glm::vec3(0.5, 0.5, 0.5),
			glm::vec3 lightP = glm::vec3(-5, 10, 0))
			:
			lightStrength(lightStr), specStrength(specStr), lightColor(lightC), lightPos(lightP)
		{}

	};
	struct ShaderAttribute
	{
		ShaderAttribute(unsigned int location,const std::string& name)
			: m_location(location),m_name(name)
		{}
		unsigned int m_location;
		std::string m_name;
	};

	struct ShaderLayout
	{
		ShaderLayout(std::initializer_list<ShaderAttribute>attributes)
			:m_attributes(attributes)
		{}
		std::vector<ShaderAttribute> m_attributes;
	};

	//class Shader
	//{
	//public:
	//	Shader(const std::string& vsSrc, const std::string& fsSrc);
	//	~Shader();
	//
	//	void SetUniforms();
	//	void AddGeometryShader(const std::string& filename);
	//	void SetLayout(const ShaderLayout& layout);
	//	void LinkShader();
	//	
	//
	//	void Update() const;
	//	void Bind() const;
	//	void UnBind() const;
	//
	//	void AddUniformBufferObject(const std::string& name,unsigned int bindIndex,unsigned int size);
	//	void UpdateUniformBufferObject(const std::string& name,const void*data,unsigned int size);
	//
	//	void AddUniform(const std::string& name);
	//
	//	void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	//	void UploadUniformVec2(const std::string& name,const glm::vec2& vector);
	//	void UploadFloat(const std::string& name, float value);
	//	void UploadInt(const std::string& name, int value);
	//public:
	//	static std::string LoadShader(const std::string& filename);
	//	static unsigned int CreateShader(const std::string& text, unsigned int shaderType);
	//
	//private:
	//	std::unordered_map<std::string, std::unique_ptr<UniformBuffer> > m_UBO;
	//	std::unordered_map<std::string, unsigned int> m_Uniforms;
	//	
	//
	//private:
	//
	//	enum
	//	{
	//		VIEW_POS_U,
	//
	//		LIGHT_STR_U,
	//
	//		SPEC_STR_U,
	//
	//		LIGHT_COLOR_U,
	//
	//		LIGHT_POS_U,
	//
	//		NUM_UNIFORMS
	//	};
	//
	//	Light BasicLight;
	//
	//	unsigned int m_program;
	//	std::vector<unsigned int> m_shaders;
	//
	//	unsigned int m_uniforms[NUM_UNIFORMS];
	//
	//
	//
	//};

	//class ComputeShader
	//{
	//public:
	//	ComputeShader(const std::string& src);
	//	~ComputeShader();
	//
	//	
	//	void Compute(unsigned int groupX,unsigned int groupY = 1,unsigned int groupZ = 1);
	//	void AddUniform(const std::string& name);
	//
	//	void UploadUniformMat4(const std::string& name, const float* data, unsigned int count = 1);
	//	void UploadUniformVec2(const std::string& name, const glm::vec2& vector);
	//	void UploadFloat(const std::string& name, float value);
	//	void UploadInt(const std::string& name, int value);
	//
	//
	//private:
	//	void LinkShader();
	//
	//private:
	//	unsigned int m_program;
	//	unsigned int m_shader;
	//	std::unordered_map<std::string, unsigned int> m_Uniforms;
	//
	//};

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