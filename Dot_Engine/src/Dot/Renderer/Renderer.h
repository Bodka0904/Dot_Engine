#pragma once
#include "Dot/Renderer/RenderCommand.h"
#include "Dot/Renderer/Shader/Shader.h"
#include "Dot/Renderer/Camera/Camera.h"
#include "Light/Light.h"


namespace Dot {

	class Renderer
	{
	public:
		
		static void Init();
		static void Clear(glm::vec4& color);

		static void BeginScene(const Ref<Shader>shader,Camera& camera);
		// Probably temporary //
		static void SubmitArraysCount(const Ref<Shader>shader, const Ref<ArrayBuffer>& vao, const glm::mat4& transform,int count, int drawMod);
		static void SubmitArraysInstanced(const Ref<Shader>shader, const Ref<Light>&light, const Ref<ArrayBuffer>& vao,unsigned int num, int drawMod);
		////////////////////////

		static void SubmitElements(const Ref<Shader>shader, const Ref<Light>&light, const Ref<ArrayBuffer>& vao, const glm::mat4& transform, int drawMod);
		static void SubmitInstances(const Ref<Shader> shader, const Ref<Light>&light, const Ref<ArrayBuffer>& vao,unsigned int num, int drawMod);
		static void EndScene(const Ref<Shader>shader);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
			glm::mat4 ViewMatrix;
			glm::mat4 ProjectionMatrix;
			glm::vec3 ViewPos;

		};
		static SceneData m_SceneData;
	};

}
