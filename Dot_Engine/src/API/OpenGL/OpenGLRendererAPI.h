#pragma once
#include "Dot/Renderer/RendererAPI.h"

namespace Dot {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void EnableDepthTest() override;
		virtual void DisableDepthTest() override;
		virtual void EnableBlend()override;
		virtual void DisableBlend()override;
		virtual void EnableAdditiveBlend() override;
		virtual void DisableAdditiveBlend() override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void SubmitArrays(const Ref<ArrayBuffer>& vao, int drawMod) override;
		virtual void SubmitArraysCount(const Ref<ArrayBuffer>& vao,int count, int drawMod) override;
		virtual void SubmitArraysInstanced(const Ref<ArrayBuffer>& vao, unsigned int num, int drawMod) override;
		virtual void SubmitElement(const Ref<ArrayBuffer>& vao, int drawMod) override;
		virtual void SubmitElementInstanced(const Ref<ArrayBuffer>& vao, unsigned int num, int drawMod) override;
		virtual void SubmitElementBase(unsigned int num,unsigned int baseIndex,unsigned int baseVertex, int drawMod) override;
	};
}