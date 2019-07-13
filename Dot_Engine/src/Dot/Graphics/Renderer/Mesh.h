#pragma once
#include "ArrayBuffer.h"

namespace Dot {
	class Mesh
	{
	public:
		Mesh(std::string filename, BufferLayout layout);
		~Mesh();

		std::shared_ptr<ArrayBuffer> GetVao();
		void SetModelMatrix(glm::mat4 modelMatrix) { m_modelMatrix = modelMatrix; }
		glm::mat4& GetModelMatrix() {return m_modelMatrix;}

	private:
		std::shared_ptr<ArrayBuffer> m_VAO;
		glm::mat4 m_modelMatrix;
		
	};
}
