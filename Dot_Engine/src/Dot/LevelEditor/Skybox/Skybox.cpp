#include "stdafx.h"
#include "Skybox.h"
#include "Dot/Renderer/Texture.h"

namespace Dot {


	Skybox::Skybox(float Size)
		: size(Size)
	{
		vao = ArrayBuffer::Create();

		float vertices[24]{
			-size,  size,  size,
			-size, -size,  size,
			 size, -size,  size,
			 size,  size,  size,
			-size,  size, -size,
			-size, -size, -size,
			 size, -size, -size,
			 size,  size, -size,
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
		vao->AddVBO(VBO);

		std::shared_ptr<IndexBuffer> IBO = IndexBuffer::Create(indices, 24);
		vao->AddIBO(IBO);
	}

	Skybox::~Skybox()
	{
		
	}

	void Skybox::Render(const Ref<Shader>& shader, int drawMod)
	{
		RenderCommand::SubmitElement(vao, drawMod);
	}

}