#include "stdafx.h"
#include "Water.h"



namespace Dot {
	Water::Water(const glm::vec3& position,const glm::vec2& size)
		:m_Height(position.y)
	{
		float vertices[] = {
			position.x,position.y,position.z,				   0,0,
			position.x + size.x,position.y,position.z,		   1,0,
			position.x + size.x,position.y,position.z + size.y,1,1,
			position.x,position.y,position.z + size.y,		   0,1
		};
		
		BufferLayout layout = {
				{0,ShaderDataType::Float3,"position"},
				{1,ShaderDataType::Float2,"texcoord"},
		};

		Ref<VertexBuffer>m_VBO = std::make_shared<VertexBuffer>(vertices,sizeof(vertices),D_STATIC_DRAW);
		m_VBO->SetLayout(layout);

		m_VAO = std::make_shared<ArrayBuffer>();
		m_VAO->AddVBO(m_VBO);


	}
}