#pragma once
#include "Dot/Debug/Log.h"
#include <glm/glm.hpp>
#include <GL/glew.h>



enum class ShaderDataType
{
	None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
};

static unsigned int ShaderDataTypeSize(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:		return 4;
	case ShaderDataType::Float2:	return 4 * 2;
	case ShaderDataType::Float3:	return 4 * 3;
	case ShaderDataType::Float4:	return 4 * 4;
	case ShaderDataType::Mat3:		return 4 * 3 * 3;
	case ShaderDataType::Mat4:		return 4 * 4 * 4;
	case ShaderDataType::Int:		return 4;
	case ShaderDataType::Int2:		return 4 * 2;
	case ShaderDataType::Int3:		return 4 * 3;
	case ShaderDataType::Int4:		return 4 * 4;
	case ShaderDataType::Bool:		return 1;
	}
	LOG_ERR("Buffer: Unknown ShaderDataType");
	return 0;
}

struct BufferElement
{
	BufferElement(ShaderDataType Type, const std::string& Name, bool Norm = false)
		:type(Type), name(Name), size(ShaderDataTypeSize(type)), offset(0), normalized(Norm)
	{};

	std::string name;
	ShaderDataType type;
	unsigned int size;
	unsigned int offset;
	bool normalized;

	unsigned int GetComponentCount() const
	{
		switch (type)
		{
		case ShaderDataType::Float:		return 1;
		case ShaderDataType::Float2:	return 2;
		case ShaderDataType::Float3:	return 3;
		case ShaderDataType::Float4:	return 4;
		case ShaderDataType::Mat3:		return 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4;
		case ShaderDataType::Int:		return 1;
		case ShaderDataType::Int2:		return 2;
		case ShaderDataType::Int3:		return 3;
		case ShaderDataType::Int4:		return 4;
		case ShaderDataType::Bool:		return 1;
		}

		LOG_ERR("Buffer: Unknown ShaderDataType");
		return 0;
	}
};

class BufferLayout
{
public:
	BufferLayout() {};
	BufferLayout(const std::initializer_list<BufferElement>& elements)
		: m_elements(elements)
	{
		CalculateOffsetsAndStride();
	};

	inline const std::vector<BufferElement>&GetElements() const { return m_elements; }
	inline const unsigned int GetStride() const { return m_stride; }

	std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
	std::vector<BufferElement>::iterator end() { return m_elements.end(); }
	std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
	std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }
private:
	void CalculateOffsetsAndStride()
	{
		unsigned int offset = 0;
		m_stride = 0;

		for (auto& element : m_elements)
		{
			element.offset = offset;
			offset += element.size;
			m_stride += element.size;
		}
	};

private:
	std::vector<BufferElement> m_elements;
	unsigned int m_stride = 0;

};


struct VertexTexture 
{
	VertexTexture(glm::vec3 Vertice, glm::vec3 Normal, glm::vec2 TexCoord)
		:vertice(Vertice), normal(Normal), texcoord(TexCoord)
	{};
	
	
	glm::vec3 vertice;
	glm::vec3 normal;
	glm::vec2 texcoord;

};

struct VertexColor
{
	VertexColor(glm::vec3 Vertice, glm::vec3 Normal, glm::vec3 Color )
		:vertice(Vertice), normal(Normal), color(Color)
	{};

	glm::vec3 vertice;
	glm::vec3 normal;
	glm::vec2 color;

};



class VertexBuffer
{
public:
	VertexBuffer(VertexTexture *vertices, unsigned int size);
	VertexBuffer(VertexColor *vertices, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;

	void SetLayout(const BufferLayout& layout) { m_layout = layout; }
	inline const BufferLayout &GetLayout() const { return m_layout; }

private:
	unsigned int m_VBO;
	BufferLayout m_layout;

};

class IndexBuffer
{
public:
	IndexBuffer(unsigned int* indices, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;

	unsigned int GetCount() const;

private:
	unsigned int m_VBO;
	unsigned int m_Count;
};



class OffsetBuffer
{
public:
	OffsetBuffer(glm::vec3* offsets,unsigned int size,unsigned int sPos);
	~OffsetBuffer();

	void Bind() const;
	void UnBind() const;

private:
	unsigned int m_VBO;


};


