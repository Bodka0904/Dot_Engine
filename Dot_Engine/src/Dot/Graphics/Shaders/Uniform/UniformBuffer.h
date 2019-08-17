#pragma once

namespace Dot {
	class UniformBuffer
	{
	public:
		UniformBuffer(const void* data, unsigned int size, unsigned int index);
		~UniformBuffer();

		void Update(const void* data,unsigned int size);
	
		unsigned int& GetUBO() { return m_UBO; }
		unsigned int& GetBlockIndex() { return m_Block_Index; }
		const unsigned int& GetIndex() const { return m_Index; }
		
	private:
		unsigned int m_UBO;
		unsigned int m_Index;
		unsigned int m_Block_Index;
		
	};

}
