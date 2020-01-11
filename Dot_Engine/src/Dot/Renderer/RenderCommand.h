#pragma once
#include "RendererAPI.h"

#define D_BLEND 0x0BE2
#define D_DEPTH_TEST 0x0B71

#define D_TRIANGLES 0x0004
#define D_POINTS 0x0000
#define D_QUADS 0x0007
#define D_LINES 0x0001


#define D_SRC_COLOR  0x0300
#define D_ONE_MINUS_SRC_COLOR 0x0301
#define D_SRC_ALPHA 0x0302
#define D_ONE_MINUS_SRC_ALPHA 0x0303


#define D_DST_ALPHA 0x0304
#define D_ONE_MINUS_DST_ALPHA 0x0305
#define D_DST_COLOR 0x0306
#define D_ONE_MINUS_DST_COLOR 0x0307


#define D_ZERO 0
#define D_FALSE 0
#define D_LOGIC_OP 0x0BF1
#define D_NONE 0
#define D_TEXTURE_COMPONENTS 0x1003
#define D_NO_ERROR 0
#define D_POINTS 0x0000
#define D_CURRENT_BIT 0x00000001
#define D_TRUE 1
#define D_ONE 1

#define D_FUNC_ADD 0x8006

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

		inline static void Enable(int32_t state)
		{
			s_RendererAPI->Enable(state);
		}
		inline static void Disable(int32_t state)
		{
			s_RendererAPI->Disable(state);
		}
		inline static void SetBlendFunc(int32_t sFactor, int32_t dFactor)
		{
			s_RendererAPI->SetBlendFunc(sFactor, dFactor);
		}
		inline static void SetBlendEquation(int32_t eq)
		{
			s_RendererAPI->SetBlendEquation(eq);
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
		inline static void SubmitArraysCount(const Ref<ArrayBuffer>& vao,int count, int drawmod)
		{
			s_RendererAPI->SubmitArraysCount(vao,count, drawmod);
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