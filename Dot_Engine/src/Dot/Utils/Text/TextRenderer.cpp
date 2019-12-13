#include "stdafx.h"
#include "TextRenderer.h"

#include <GL/glew.h>

namespace Dot {
	TextRenderer::SceneData TextRenderer::m_SceneData;
	void TextRenderer::BeginRender(const Ref<OrthoCamera> camera)
	{
		m_SceneData.ViewProjectionMatrix = camera->GetViewProjectionMatrix();
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);
	}

	void TextRenderer::Render(const Ref<Shader> shader, const Ref<ArrayBuffer>& vao, int count)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjectionMatrix", m_SceneData.ViewProjectionMatrix);
		vao->Bind();

		glDrawArrays(GL_QUADS, 0, count);
	}

	void TextRenderer::EndRender()
	{
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_COLOR, GL_SRC_COLOR);
		glEnable(GL_DEPTH_TEST);
	}
}