#include "stdafx.h"
#include "WaterTile.h"

#include "Dot/Renderer/Renderable/Renderable.h"
#include "Dot/Renderer/RenderCommand.h"

namespace Dot {
	WaterTile::WaterTile(const glm::vec3& position, const glm::vec2& size)
		: m_Position(position),m_Size(size)
	{
		m_VAO = ArrayBuffer::Create();
		BufferLayout layout = {
				{0,ShaderDataType::Float3,"a_Position"},
				{1,ShaderDataType::Float2,"a_TexCoord"},
		};

		Vertex3D vertex[4];
		vertex[0].position=(glm::vec3(position.x, position.y, position.z));
		vertex[1].position=(glm::vec3(position.x + size.x, position.y, position.z));
		vertex[2].position=(glm::vec3(position.x + size.x, position.y, position.z + size.y));
		vertex[3].position=(glm::vec3(position.x, position.y, position.z + size.y));

		vertex[0].texcoord = glm::vec2(0, 0);
		vertex[1].texcoord = glm::vec2(1, 0);
		vertex[2].texcoord = glm::vec2(1, 1);
		vertex[3].texcoord = glm::vec2(0, 1);

		Ref<VertexBuffer> VBO = VertexBuffer::Create((float*)& vertex[0], sizeof(Vertex3D) * 4, D_DYNAMIC_DRAW);
		VBO->SetLayout(layout);
		m_VAO->AddVBO(VBO);
		
		unsigned int indices[6] = {
			0,1,2,0,2,3
		};

		Ref<IndexBuffer> IBO = IndexBuffer::Create(&indices[0], 6);
		m_VAO->AddIBO(IBO);
	}
	void WaterTile::Draw()
	{
		RenderCommand::SubmitElement(m_VAO, D_TRIANGLES);
	}
}