#pragma once
#include "Dot/Debug/Log.h"
#include <glm/glm.hpp>

#define D_STATIC_DRAW 0x88E4
#define D_DYNAMIC_DRAW 0x88E8
#define D_STREAM_DRAW 0x88E0

namespace Dot {

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
		BufferElement(unsigned int Index,ShaderDataType Type, const std::string& Name, unsigned int Divisor = 0, bool Norm = false)
			:index(Index),type(Type), name(Name), size(ShaderDataTypeSize(type)), divisor(Divisor), offset(0),  normalized(Norm)
		{
		};

		std::string name;
		ShaderDataType type;
		unsigned int size;
		unsigned int offset;
		unsigned int index;
		unsigned int divisor;
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
			CreateMat4();
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
		

		void CreateMat4()
		{
			for (auto& element : m_elements)
			{
				switch (element.type)
				{
				case ShaderDataType::Mat4:
				{
					element.type = ShaderDataType::Float4;
					element.size = 4 * 4;

					BufferElement tmpElement = element;
					m_elements.push_back(BufferElement(tmpElement.index + 1, tmpElement.type, tmpElement.name, tmpElement.divisor));
					m_elements.push_back(BufferElement(tmpElement.index + 2, tmpElement.type, tmpElement.name, tmpElement.divisor));
					m_elements.push_back(BufferElement(tmpElement.index + 3, tmpElement.type, tmpElement.name, tmpElement.divisor));
				}
				}
			}
		}


	private:
		std::vector<BufferElement> m_elements;
		unsigned int m_stride = 0;

	};

	
	
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual void Update(const void *vertices,unsigned int size, int offset) = 0;
		virtual void* MapBuffer() = 0;
		virtual void  UnMapBuffer() = 0;
		virtual void ClearBuffer() = 0;
		virtual void Resize(const void* vertices, unsigned int size) = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;

		virtual const BufferLayout &GetLayout() const = 0;

		static Ref<VertexBuffer> Create(const void* vertices, unsigned int size, int drawMod);


	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual unsigned int GetCount() const = 0;

		static Ref<IndexBuffer> Create(const void* indices, unsigned int count);
	};

	class ShaderStorageBuffer
	{
	public:
		virtual ~ShaderStorageBuffer() = default;

		virtual void BindBase(unsigned int point) = 0;
		virtual void BindRange(unsigned int index) = 0;
		virtual void Bind() = 0;
		virtual void Update(const void* vertices, unsigned int size, int offset) = 0;
		virtual void Resize(const void* vertices, unsigned int size) = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout & GetLayout() const = 0;

		static Ref<ShaderStorageBuffer> Create(const void* vertices, unsigned int size, int drawMod);
	
	};

}