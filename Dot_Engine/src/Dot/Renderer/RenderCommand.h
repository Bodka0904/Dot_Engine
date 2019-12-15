#pragma once
#include "RendererAPI.h"

#define D_TRIANGLES 0x0004
#define D_POINTS 0x0000
#define D_QUADS 0x0007
#define D_LINES 0x0001

namespace Dot {

	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		// View setup
		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		// Render setup
		inline static void EnableDepthTest()
		{
			s_RendererAPI->EnableDepthTest();
		}
		inline static void DisableDepthTest()
		{
			s_RendererAPI->DisableDepthTest();
		}
		inline static void EnableBlend()
		{
			s_RendererAPI->EnableBlend();
		}
		inline static void DisableBlend()
		{
			s_RendererAPI->DisableBlend();
		}

		// Render options
		inline static void SubmitElement(const Ref<ArrayBuffer>& vao, int drawMod)
		{
			s_RendererAPI->SubmitElement(vao,drawMod);
		}
		inline static void SubmitElementInstanced(const Ref<ArrayBuffer>& vao, unsigned int num, int drawMod)
		{
			s_RendererAPI->SubmitElementInstanced(vao, num, drawMod);
		}
		inline static void SubmitArrays(const Ref<ArrayBuffer>& vao, int drawmod)
		{
			s_RendererAPI->SubmitArrays(vao, drawmod);
		}
		inline static void SubmitArraysInstanced(const Ref<ArrayBuffer>& vao, unsigned int num, int drawMod)
		{
			s_RendererAPI->SubmitArraysInstanced(vao, num, drawMod);
		}
		inline static void SubmitElementBase(unsigned int num, unsigned int baseIndex, unsigned int baseVertex, int drawMod)
		{
			s_RendererAPI->SubmitElementBase(num,baseIndex,baseVertex, drawMod);
		}
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};
}