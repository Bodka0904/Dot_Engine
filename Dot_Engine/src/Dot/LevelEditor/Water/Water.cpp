#include "stdafx.h"
#include "Water.h"


#include "Dot/Renderer/RenderCommand.h"

namespace Dot {

	Water::Water(const glm::vec3& position,const glm::vec3& color,const glm::vec2& size,const float vertnum)
		:m_Height(position.y),m_Color(color)
	{
		Ref<ArrayBuffer>VAO;
		std::vector<glm::vec3> positions;
		std::vector<unsigned int> indices;

		BufferLayout layout = {
				{0,ShaderDataType::Float3,"position"},
		};

		for (int i = 0; i < vertnum; ++i)
		{
			for (int j = 0; j < vertnum; ++j)
			{
				positions.push_back(glm::vec3(position.x + (i * size.x), position.y, position.z + (j * size.y)));
			}
		}

		for (int j = 0; j < vertnum - 1; ++j)
		{
			for (int i = 0; i < vertnum - 1; ++i)
			{
				int start = j * vertnum + i;
				indices.push_back(start);
				indices.push_back(start + 1);
				indices.push_back(start + vertnum);

				indices.push_back(start + 1);
				indices.push_back(start + 1 + vertnum);
				indices.push_back(start + vertnum);

			}
		}

		Ref<VertexBuffer>m_VBO = VertexBuffer::Create((float*)positions.data(),positions.size()*sizeof(glm::vec3), D_STATIC_DRAW);
		m_VBO->SetLayout(layout);

		VAO = ArrayBuffer::Create();
		VAO->AddVBO(m_VBO);

		Ref<IndexBuffer>m_IBO = IndexBuffer::Create((void*)&indices[0], indices.size());
		VAO->AddIBO(m_IBO);

		m_Mesh = std::make_shared<Mesh>(VAO);
	}

	void Water::Update(float dt)
	{
		if (m_TimePass >= 1)
		{
			m_TimePass = -1;
		}
		m_TimePass += dt;
	}

	void Water::Render(const Ref<Shader>& shader) 
	{
		m_Mesh->Render(shader, D_TRIANGLES);
	}
	
}