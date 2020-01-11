#pragma once

namespace Dot {
	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default;

		virtual void Update(const void* data,unsigned int size, int offset) = 0;

		static Ref<UniformBuffer> Create(const void* data, unsigned int size, unsigned int index);
	};

}
