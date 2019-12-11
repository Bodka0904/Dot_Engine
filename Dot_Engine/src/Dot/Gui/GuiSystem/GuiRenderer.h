#pragma once
#include "Dot/Renderer/Shaders/Shader.h"
#include "Dot/Renderer/Camera/OrthoCamera.h"
#include "Dot/Renderer/Buffers/ArrayBuffer.h"


namespace Dot {
	class GuiRenderer
	{
	public:
		static void BeginRender(const Ref<OrthoCamera> camera);
		static void Render(const Ref<Shader>shader, const Ref<ArrayBuffer>& vao);
		static void EndRender();

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData m_SceneData;

	};


}