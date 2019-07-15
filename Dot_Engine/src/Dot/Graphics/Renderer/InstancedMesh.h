#pragma once
#include "ArrayBuffer.h"
#include <glm/glm.hpp>

namespace Dot {
	class InstancedMesh
	{
	public:
		InstancedMesh(const std::string& filename, BufferLayout layout, std::vector<glm::mat4> matrices);
		~InstancedMesh();

		std::shared_ptr<ArrayBuffer> GetVao();
		const unsigned int &GetNum() const { return m_num; }
	private:
		std::shared_ptr<ArrayBuffer> m_VAO;
		unsigned int m_num;
	};

}