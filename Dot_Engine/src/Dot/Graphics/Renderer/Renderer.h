#pragma once
#include "Dot/Graphics/Shaders/Shader.h"
#include "Dot/Graphics/Renderer/Camera/Camera.h"
#include "Dot/Graphics/Transform.h"
#include "Mesh.h"
#include "InstancedMesh.h"

namespace Dot {

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		static void Clear(glm::vec4& color);

		static void BeginScene(Camera& camera);
		static void SubmitArrays(const std::shared_ptr<Shader>shader ,const std::shared_ptr<ArrayBuffer>& vao);
		static void SubmitElements(const std::shared_ptr<Shader>shader,const std::shared_ptr<Mesh>& mesh);
		static void SubmitInstances(const std::shared_ptr<Shader> shader, const std::shared_ptr<InstancedMesh>& mesh);
		static void EndScene(const std::shared_ptr<Shader>shader);
	
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
			glm::mat4 ViewMatrix;
			glm::mat4 ProjectionMatrix;

		};

		static SceneData* m_SceneData;
	};

}

