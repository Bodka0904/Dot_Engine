#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>


enum class  GuiShaderDataType
{
	None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
};

struct  GuiBufferElement
{
	GuiBufferElement(GuiShaderDataType Type, const std::string& Name, bool Norm = false)
		:type(Type), name(Name)
	{};

	std::string name;
	GuiShaderDataType type;

};

class GuiBufferLayout
{
public:
	GuiBufferLayout() {};
	GuiBufferLayout(const std::initializer_list< GuiBufferElement>& elements)
		: m_elements(elements)
	{	
	};

	inline const std::vector< GuiBufferElement>&GetElements() const { return m_elements; }
	
private:
	std::vector< GuiBufferElement> m_elements;
	

};

struct GuiVertex
{
	GuiVertex(glm::vec2 Vertice, glm::vec4 Color)
		:vertice(Vertice), color(Color)
	{};

	
	glm::vec2 vertice;
	glm::vec4 color;

};


class GuiVertexBuffer
{
public:
	GuiVertexBuffer(GuiVertex *vertices, unsigned int size);
	~GuiVertexBuffer();

	void Bind() const;
	void UnBind() const;

	void SetLayout(const  GuiBufferLayout& layout) { m_layout = layout; }
	inline const  GuiBufferLayout &GetLayout() const { return m_layout; }

	unsigned int &GetVBO() { return m_VBO; }
private:
	unsigned int m_VBO;
	GuiBufferLayout m_layout;
};

class GuiIndexBuffer
{
public:
	GuiIndexBuffer(unsigned int* indices, unsigned int count);
	~GuiIndexBuffer();

	void Bind() const;
	void UnBind() const;

	unsigned int GetCount() const;
	unsigned int &GetVBO() { return m_VBO; }
private:
	unsigned int m_VBO;
	unsigned int m_Count;
};