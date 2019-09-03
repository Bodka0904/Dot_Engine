#include "stdafx.h"
#include "Renderer.h"
#include "Dot/Debug/Timer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Dot {

	Renderer::SceneData Renderer::m_SceneData;

	Renderer::Renderer()
	{
	}


	Renderer::~Renderer()
	{

	}

	void Renderer::Clear(glm::vec4& color)
	{
		glClearColor(color.x, color.y, color.z, color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	}

	void Renderer::BeginScene(Camera& camera)
	{
		m_SceneData.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		m_SceneData.ViewMatrix = camera.GetViewMatrix();
		m_SceneData.ProjectionMatrix = camera.GetProjectionMatrix();

	}

	void Renderer::SubmitArrays(const Ref<Shader> shader, const Ref<ArrayBuffer>& vao, const glm::mat4& transform, int drawMod)
	{
		shader->Bind();
		shader->Update();
		vao->Bind();
		glDrawArrays(drawMod, 0, vao->GetVertexBuffer(0)->GetCount());
	}

	void Renderer::SubmitArraysInstanced(const Ref<Shader> shader, const Ref<ArrayBuffer>& vao,unsigned int num, int drawMod)
	{
		shader->Bind();
		shader->Update();
		vao->Bind();
		glDrawArraysInstanced(drawMod, 0, 1, num);

	}


	void Renderer::SubmitElements(const Ref<Shader>shader, const Ref<Mesh>& mesh, const glm::mat4& transform, int drawMod)
	{
		shader->Bind();
		shader->Update();
		mesh->GetVao()->Bind();

	

		shader->UploadUniformMat4("ModelMatrix",transform);


		glDrawElements(drawMod, mesh->GetVao()->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
	}


	void Renderer::SubmitInstances(const Ref<Shader> shader, const Ref<InstancedMesh>& mesh, int drawMod)
	{

		shader->Bind();
		shader->Update();
		mesh->GetVao()->Bind();
		glDrawElementsInstanced(drawMod, mesh->GetVao()->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0, mesh->GetNum());

	}

	void Renderer::EndScene(const Ref<Shader>shader)
	{
		shader->UpdateUniformBufferObject("camera_data", &m_SceneData, sizeof(glm::mat4) * 3);
	}

}