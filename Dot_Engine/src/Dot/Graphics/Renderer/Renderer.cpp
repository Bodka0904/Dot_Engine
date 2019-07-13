#include "stdafx.h"
#include "Renderer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Dot {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	Renderer::Renderer()
	{
	}


	Renderer::~Renderer()
	{
	}

	void Renderer::Clear(glm::vec4 & color)
	{
		glClearColor(color.x, color.y, color.z, color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::BeginScene(Camera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::Submit(const std::shared_ptr<Shader>shader, const Transform& transform, const std::shared_ptr<Mesh>& mesh)
	{

		mesh->SetModelMatrix(transform.GetModel());
		shader->Bind();
		shader->Update();	
		shader->UploadUniformMat4("ModelMatrix", mesh->GetModelMatrix());
		shader->UploadFloat("time", glfwGetTime());
		mesh->GetVao()->Bind();
		glDrawElements(GL_TRIANGLES, mesh->GetVao()->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
	
	}

	void Renderer::EndScene(const std::shared_ptr<Shader>shader)
	{
		shader->UpdateUniformBufferObject("camera_data", m_SceneData);
	}
}