#include "stdafx.h"
#include "Renderer.h"
#include "Dot/Debug/Timer.h"


namespace Dot {
	Renderer::SceneData Renderer::m_SceneData;


	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::Clear(glm::vec4& color)
	{
		RenderCommand::Clear();
		RenderCommand::SetClearColor(color);
	}

	void Renderer::BeginScene(Camera& camera)
	{
		m_SceneData.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		m_SceneData.ViewMatrix = camera.GetViewMatrix();
		m_SceneData.ProjectionMatrix = camera.GetProjectionMatrix();
		m_SceneData.ViewPos = camera.GetPosition();
	}

	void Renderer::SubmitArraysCount(const Ref<Shader> shader, const Ref<ArrayBuffer>& vao, const glm::mat4& transform,int count, int drawMod)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ModelMatrix", transform);

		RenderCommand::SubmitArraysCount(vao,count, drawMod);
	}
	void Renderer::SubmitArraysInstanced(const Ref<Shader> shader, const Ref<Light>&light, const Ref<ArrayBuffer>& vao,unsigned int num, int drawMod)
	{
		shader->Bind();
		shader->UploadUniformFloat3("u_LightPosition", light->GetPosition());
		shader->UploadUniformFloat3("u_LightColor", light->GetColor());
		shader->UploadUniformFloat("u_LightStrength", light->GetStrength());

		RenderCommand::SubmitArraysInstanced(vao, num, drawMod);
	}


	void Renderer::SubmitElements(const Ref<Shader> shader, const Ref<Light>&light, const Ref<ArrayBuffer>& vao, const glm::mat4& transform, int drawMod)
	{
		shader->Bind();

		shader->UploadUniformMat4("u_ModelMatrix", transform);
		shader->UploadUniformFloat3("u_LightPosition", light->GetPosition());
		shader->UploadUniformFloat3("u_LightColor", light->GetColor());
		shader->UploadUniformFloat("u_LightStrength", light->GetStrength());

		RenderCommand::SubmitElement(vao, drawMod);
	}



	void Renderer::SubmitInstances(const Ref<Shader> shader, const Ref<Light>&light, const Ref<ArrayBuffer>& vao, unsigned int num, int drawMod)
	{

		shader->Bind();
		shader->UploadUniformFloat3("u_LightPosition", light->GetPosition());
		shader->UploadUniformFloat3("u_LightColor", light->GetColor());
		shader->UploadUniformFloat("u_LightStrength", light->GetStrength());
		
		RenderCommand::SubmitElementInstanced(vao, num, drawMod);
	}

	void Renderer::EndScene(const Ref<Shader>shader)
	{
		shader->UpdateUniformBufferObject("o_CameraData", &m_SceneData, (sizeof(glm::mat4) * 3) + sizeof(glm::vec3));
	}

}