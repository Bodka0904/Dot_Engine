#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>




	enum class  GuiShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static unsigned int ShaderDataTypeSize(GuiShaderDataType type)
	{
		switch (type)
		{
		case  GuiShaderDataType::Float:		return 4;
		case  GuiShaderDataType::Float2:	return 4 * 2;
		case  GuiShaderDataType::Float3:	return 4 * 3;
		case  GuiShaderDataType::Float4:	return 4 * 4;
		case  GuiShaderDataType::Mat3:		return 4 * 3 * 3;
		case  GuiShaderDataType::Mat4:		return 4 * 4 * 4;
		case  GuiShaderDataType::Int:		return 4;
		case  GuiShaderDataType::Int2:		return 4 * 2;
		case  GuiShaderDataType::Int3:		return 4 * 3;
		case  GuiShaderDataType::Int4:		return 4 * 4;
		case  GuiShaderDataType::Bool:		return 1;
		}
		printf("Buffer: Unknown ShaderDataType");
		return 0;
	}

	struct  GuiBufferElement
	{
		GuiBufferElement(unsigned int Index, GuiShaderDataType Type, const std::string& Name, unsigned int Divisor = 0, bool Norm = false)
			:index(Index), type(Type), name(Name), size(ShaderDataTypeSize(type)), divisor(Divisor), offset(0), normalized(Norm)
		{
		};

		std::string name;
		GuiShaderDataType type;
		unsigned int size;
		unsigned int offset;
		unsigned int index;
		unsigned int divisor;
		bool normalized;

		unsigned int GetComponentCount() const
		{
			switch (type)
			{
			case  GuiShaderDataType::Float:		return 1;
			case  GuiShaderDataType::Float2:	return 2;
			case  GuiShaderDataType::Float3:	return 3;
			case  GuiShaderDataType::Float4:	return 4;
			case  GuiShaderDataType::Mat3:		return 3 * 3;
			case  GuiShaderDataType::Mat4:		return 4 * 4;
			case  GuiShaderDataType::Int:		return 1;
			case  GuiShaderDataType::Int2:		return 2;
			case  GuiShaderDataType::Int3:		return 3;
			case  GuiShaderDataType::Int4:		return 4;
			case  GuiShaderDataType::Bool:		return 1;
			}

			printf("Buffer: Unknown ShaderDataType");
			return 0;
		}

	};

	class  GuiBufferLayout
	{
	public:
		GuiBufferLayout() {};
		GuiBufferLayout(const std::initializer_list< GuiBufferElement>& elements)
			: m_elements(elements)
		{
			CreateMat4();
			CalculateOffsetsAndStride();
		};

		inline const std::vector< GuiBufferElement>& GetElements() const { return m_elements; }
		inline const unsigned int GetStride() const { return m_stride; }

		std::vector< GuiBufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector< GuiBufferElement>::iterator end() { return m_elements.end(); }
		std::vector< GuiBufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector< GuiBufferElement>::const_iterator end() const { return m_elements.end(); }
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


		void CreateMat4()
		{
			for (auto& element : m_elements)
			{
				switch (element.type)
				{
				case  GuiShaderDataType::Mat4:
				{
					element.type = GuiShaderDataType::Float4;
					element.size = 4 * 4;

					GuiBufferElement tmpElement = element;
					m_elements.push_back( GuiBufferElement(tmpElement.index + 1, tmpElement.type, tmpElement.name, tmpElement.divisor));
					m_elements.push_back( GuiBufferElement(tmpElement.index + 2, tmpElement.type, tmpElement.name, tmpElement.divisor));
					m_elements.push_back( GuiBufferElement(tmpElement.index + 3, tmpElement.type, tmpElement.name, tmpElement.divisor));
				}
				}
			}
		}


	private:
		std::vector< GuiBufferElement> m_elements;
		unsigned int m_stride = 0;

	};


	struct  GuiVertex
	{
		GuiVertex() {};
		GuiVertex(glm::vec2 Vertice, glm::vec2 TexCoord)
			:vertice(Vertice), texcoord(TexCoord)
		{};


		glm::vec2 vertice;
		glm::vec2 texcoord;

	};

	struct  GuiVertexColor
	{
		GuiVertexColor(glm::vec2 Vertice, glm::vec3 Color)
			:vertice(Vertice), color(Color)
		{};

		glm::vec2 vertice;
		glm::vec3 color;

	};



	class  GuiVertexBuffer
	{
	public:
		GuiVertexBuffer(const void* vertices, unsigned int size,bool dynamic = true);
		~GuiVertexBuffer();

		void Bind() const;
		void UnBind() const;


		void Update(const void* vertices, unsigned int size);

		void SetLayout(const  GuiBufferLayout& layout) { m_layout = layout; }
		inline const  GuiBufferLayout& GetLayout() const { return m_layout; }

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

	private:
		unsigned int m_VBO;
		unsigned int m_Count;
	};




