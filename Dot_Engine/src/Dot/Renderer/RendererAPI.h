#pragma once
#include <glm/glm.hpp>
#include "Dot/Renderer/Buffers/ArrayBuffer.h"

namespace Dot {
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
	public:
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void Enable(int32_t state) = 0;
		virtual void Disable(int32_t state) = 0;
		virtual void SetBlendFunc(int32_t sFactor, int32_t dFactor) = 0;
		virtual void SetBlendEquation(int32_t eq) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void ClearBuffer() = 0;

		virtual void SubmitArraysCount(const Ref<ArrayBuffer>& vao,int count, int drawMod) = 0;
		virtual void SubmitArraysInstanced(const Ref<ArrayBuffer>& vao, unsigned int num, int drawMod) = 0;
		virtual void SubmitElement(const Ref<ArrayBuffer>& vao, int drawMod) = 0;
		virtual void SubmitElementInstanced(const Ref<ArrayBuffer>& vao, unsigned int num, int drawMod) = 0;
		virtual void SubmitElementBase(unsigned int num, unsigned int baseIndex, unsigned int baseVertex, int drawMod) = 0;
		inline static API GetAPI() { return s_API; }
		static Scope<RendererAPI> Create();
	private:
		static API s_API;
	
	};
}