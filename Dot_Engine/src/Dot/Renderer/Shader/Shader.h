#pragma once

#include "Dot/Debug/Log.h"

#include <conio.h>
#include <glm/glm.hpp>
#include <queue>

namespace Dot {
	
	enum class UniformDataType
	{
		NONE,
		SAMPLER2D,
		FLOAT,FLOAT_VEC2,FLOAT_VEC3,FLOAT_VEC4,
		INT,INT_VEC2,INT_VEC3,INT_VEC4,
		FLOAT_MAT4,
		STRUCT
	};

	struct Uniform
	{
		UniformDataType type;
		int ID;
		unsigned int offset;
		unsigned int size;
	};

	class Shader
	{
	public:
		~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Compute(unsigned int groupX, unsigned int groupY = 1, unsigned int groupZ = 1) const = 0;
		virtual void Unbind() const = 0;
		virtual void SetUniform(const std::string& name, unsigned char* data) = 0;
	
		virtual const std::string& GetName() const = 0;
		virtual const uint32_t GetRendererID() const = 0;
		virtual const Uniform* FindUniform(const std::string& name) = 0;
		
	
		virtual void UpdateUniformBufferObject(const std::string& name, const void* data, unsigned int size, int offset = 0) = 0;
		virtual void UploadUniformInt(const std::string& name, int value) = 0;
		
		virtual void UploadUniformFloat(const std::string& name, float value) = 0;
		virtual void UploadUniformFloat2(const std::string& name, const glm::vec2& value)= 0;
		virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& value)= 0;
		virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& value)= 0;
	 
		virtual void UploadUniformMat3(const std::string& name, const glm::mat3& matrix)= 0;
		virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix)= 0;
		virtual unsigned int GetUniformSize() = 0;
		
		static Ref<Shader> Create(const std::string& name, const std::string& filepath);
	private:
		virtual void LinkUniformBufferObject(const std::string& name, unsigned int bindIndex, unsigned int size) = 0;
		virtual void AddUniform(UniformDataType type, unsigned int size, unsigned int offset, const std::string& name) = 0;

	};


}