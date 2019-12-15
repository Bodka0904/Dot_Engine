#include "stdafx.h"
#include "Skybox.h"
#include "Dot/Renderer/Texture.h"
#include "Dot/Renderer/Mesh.h"

#include <GL/glew.h>

namespace Dot {


	Skybox::Skybox(const std::vector<std::string> faces,int SIZE)
	{
		m_CubeMap = CubeMapTexture::Create(faces);

		float vertices[108] = {
		-SIZE,  SIZE, -SIZE,
		-SIZE, -SIZE, -SIZE,
		 SIZE, -SIZE, -SIZE,
		 SIZE, -SIZE, -SIZE,
		 SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,

		-SIZE, -SIZE,  SIZE,
		-SIZE, -SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE,  SIZE,
		-SIZE, -SIZE,  SIZE,

		 SIZE, -SIZE, -SIZE,
		 SIZE, -SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE, -SIZE,
		 SIZE, -SIZE, -SIZE,

		-SIZE, -SIZE,  SIZE,
		-SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE, -SIZE,  SIZE,
		-SIZE, -SIZE,  SIZE,

		-SIZE,  SIZE, -SIZE,
		 SIZE,  SIZE, -SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		-SIZE,  SIZE,  SIZE,
		-SIZE,  SIZE, -SIZE,

		-SIZE, -SIZE, -SIZE,
		-SIZE, -SIZE,  SIZE,
		 SIZE, -SIZE, -SIZE,
		 SIZE, -SIZE, -SIZE,
		-SIZE, -SIZE,  SIZE,
		 SIZE, -SIZE,  SIZE
		};

		BufferLayout layout = {
			{0, Dot::ShaderDataType::Float3, "position" }
		};

		std::shared_ptr<VertexBuffer> m_VBO;
		m_VBO = VertexBuffer::Create(vertices, sizeof(vertices), D_STATIC_DRAW);
		m_VBO->SetLayout(layout);

		m_VAO = ArrayBuffer::Create();
		m_VAO->AddVBO(m_VBO);
	}

	Skybox::~Skybox()
	{
		
	}

}