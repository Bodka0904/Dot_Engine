#pragma once
#include "Dot/Graphics/Shaders/Shader.h"
#include "Dot/Graphics/Renderer/Camera/OrthoCamera.h"
#include "Dot/Graphics/Renderer/Buffers/ArrayBuffer.h"

namespace Dot {
	class TextRenderer
	{
	public:
		static void BeginRender(const Ref<OrthoCamera> camera);
		static void Render(const Ref<Shader>shader, const Ref<ArrayBuffer>& vao);
		static void RenderDynamic(const Ref<Shader>shader, const Ref<ArrayBuffer>& vao, int count);
		static void EndRender();

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};


		static SceneData m_SceneData;
	};
}