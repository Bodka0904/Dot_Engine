#pragma once
#include <conio.h>
#include <map>
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

	class Shader
	{
	public:
		Shader(const std::string& filename);
		virtual ~Shader();

		virtual void SetUniforms();
		virtual void AddGeometryShader(const std::string& filename);
		virtual void SetLayout(const ShaderLayout& layout);
		virtual void LinkShader();
		virtual void Clean();

		virtual void Update() const;
		virtual void Bind() const;
		virtual void UnBind() const;


		virtual void AddUniformBufferObject(const std::string& name,unsigned int bindIndex,unsigned int size);
		virtual void UpdateUniformBufferObject(const std::string& name,const void*data);

		virtual void AddUniform(const std::string& name);
		virtual void UploadUniformMat4(const std::string& name,const glm::mat4& matrix);
		virtual void UploadFloat(const std::string& name, float value);
	private:
		static std::string LoadShader(const std::string& filename);
		static unsigned int CreateShader(const std::string& text, unsigned int shaderType);

	private:
		std::map<std::string, std::unique_ptr<UniformBuffer> > m_UBO;
		std::map<std::string, unsigned int> m_Uniforms;
		

	private:

		enum
		{
			VIEW_POS_U,

			LIGHT_STR_U,

			SPEC_STR_U,

			LIGHT_COLOR_U,

			LIGHT_POS_U,

			NUM_UNIFORMS
		};

		Light BasicLight;

		unsigned int m_program;
		std::vector<unsigned int> m_shaders;
	
		unsigned int m_computeShader;
		unsigned int m_uniforms[NUM_UNIFORMS];



	};

}