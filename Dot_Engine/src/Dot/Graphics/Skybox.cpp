#include "stdafx.h"
#include "Skybox.h"
#include "Renderer/Texture.h"
#include "Renderer/Mesh.h"
#include <GL/glew.h>

namespace Dot {


	Skybox::Skybox(const std::vector<std::string> faces,int SIZE)
	{
		m_CubeMap = std::make_shared<CubeMapTexture>(faces);

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
		m_VBO = std::make_shared<VertexBuffer>(vertices, sizeof(vertices), D_STATIC_DRAW);
		m_VBO->SetLayout(layout);

		m_VAO = std::make_shared<ArrayBuffer>();
		m_VAO->AddVBO(m_VBO);
	}

	Skybox::~Skybox()
	{
		
	}

}