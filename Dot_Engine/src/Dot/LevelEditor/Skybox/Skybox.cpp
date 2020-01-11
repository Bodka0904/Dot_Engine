#include "stdafx.h"
#include "Skybox.h"
#include "Dot/Renderer/Texture.h"

namespace Dot {


	Skybox::Skybox(const std::vector<std::string> faces,float SIZE)
	{
		m_CubeMap = CubeMapTexture::Create(faces);
		m_VAO = ArrayBuffer::Create();

		float vertices[24]{
			-SIZE,  SIZE,  SIZE,
			-SIZE, -SIZE,  SIZE,
			 SIZE, -SIZE,  SIZE,
			 SIZE,  SIZE,  SIZE,
			-SIZE,  SIZE, -SIZE,
			-SIZE, -SIZE, -SIZE,
			 SIZE, -SIZE, -SIZE,
			 SIZE,  SIZE, -SIZE,
		};

		unsigned int indices[24] = {
			 0, 1, 2, 3,
			 3, 2, 6, 7,
			 7, 6, 5, 4,
			 4, 5, 1, 0,
			 0, 3, 7, 4,
			 1, 2, 6, 5,
		}; 

		BufferLayout layout = {
			{0, Dot::ShaderDataType::Float3, "position" }
		};
		std::shared_ptr<VertexBuffer> VBO = VertexBuffer::Create(vertices, sizeof(vertices), D_STATIC_DRAW);
		VBO->SetLayout(layout);
		m_VAO->AddVBO(VBO);

		std::shared_ptr<IndexBuffer> IBO = IndexBuffer::Create(indices, 24);
		m_VAO->AddIBO(IBO);
	}

	Skybox::~Skybox()
	{
		
	}

}