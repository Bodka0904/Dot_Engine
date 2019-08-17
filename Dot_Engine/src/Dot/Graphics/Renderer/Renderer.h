#pragma once
#include "Dot/Graphics/Shaders/Shader.h"
#include "Dot/Graphics/Renderer/Camera/Camera.h"
#include "Dot/Graphics/Transform.h"
#include "Mesh.h"


namespace Dot {

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		static void Clear(glm::vec4& color);

		static void BeginScene(Camera& camera);
		static void SubmitArrays(const Ref<Shader>shader ,const Ref<ArrayBuffer>& vao);
		static void SubmitElements(const Ref<Shader>shader,const Ref<Mesh>& mesh);
		static void SubmitInstances(const Ref<Shader> shader, const Ref<InstancedMesh>& mesh);
		static void EndScene(const Ref<Shader>shader);


	
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
			glm::mat4 ViewMatrix;
			glm::mat4 ProjectionMatrix;

		};


		static SceneData m_SceneData;
	};

}

