#include "stdafx.h"
#include "TextRenderer.h"

#include "RenderCommand.h"


namespace Dot {
	TextRenderer::SceneData TextRenderer::m_SceneData;
	void TextRenderer::BeginRender(const Ref<OrthoCamera> camera)
	{
		m_SceneData.ViewProjectionMatrix = camera->GetViewProjectionMatrix();
		
		RenderCommand::EnableBlend();
		RenderCommand::DisableDepthTest();
	}

	void TextRenderer::Render(const Ref<Shader> shader, const Ref<ArrayBuffer>& vao, int count)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjectionMatrix", m_SceneData.ViewProjectionMatrix);

		RenderCommand::SubmitArrays(vao, D_QUADS);
	}

	void TextRenderer::EndRender()
	{
		RenderCommand::EnableDepthTest();
	}
}