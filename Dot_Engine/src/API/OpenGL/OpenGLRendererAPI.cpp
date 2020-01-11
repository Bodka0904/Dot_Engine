#include "stdafx.h"
#include "OpenGLRendererAPI.h"
#include <GL/glew.h>

namespace Dot {
	void OpenGLRendererAPI::Init()
	{	
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
	}
	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}
	void OpenGLRendererAPI::Enable(int32_t state)
	{
		glEnable(state);
	}
	void OpenGLRendererAPI::Disable(int32_t state)
	{
		glDisable(state);
	}
	void OpenGLRendererAPI::SetBlendFunc(int32_t sFactor, int32_t dFactor)
	{
		glBlendFunc(sFactor, dFactor);
	}

	void OpenGLRendererAPI::SetBlendEquation(int32_t eq)
	{
		glBlendEquation(eq);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void OpenGLRendererAPI::SubmitArrays(const Ref<ArrayBuffer>& vao, int drawMod)
	{
		vao->Bind();
		glDrawArrays(drawMod, 0, vao->GetVertexBuffer(0)->GetCount());
	}
	void OpenGLRendererAPI::SubmitArraysCount(const Ref<ArrayBuffer>& vao,int count, int drawMod)
	{
		vao->Bind();
		glDrawArrays(drawMod, 0, count);
	}
	void OpenGLRendererAPI::SubmitArraysInstanced(const Ref<ArrayBuffer>& vao, unsigned int num, int drawMod)
	{
		vao->Bind();
		
		glDrawArraysInstanced(drawMod, 0, 4, num);
	}
	void OpenGLRendererAPI::SubmitElement(const Ref<ArrayBuffer>& vao, int drawMod)
	{
		vao->Bind();
		glDrawElements(drawMod, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
	}
	void OpenGLRendererAPI::SubmitElementInstanced(const Ref<ArrayBuffer>& vao, unsigned int num, int drawMod)
	{
		vao->Bind();
		glDrawElementsInstanced(drawMod, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0, num);
	}
	void OpenGLRendererAPI::SubmitElementBase(unsigned int num, unsigned int baseIndex, unsigned int baseVertex, int drawMod)
	{
		glDrawElementsBaseVertex(drawMod,num,GL_UNSIGNED_INT,(void*)(sizeof(unsigned int) * baseIndex),baseVertex);
	}
}