#pragma once
#include "Dot/ECS/ECSManager.h"
#include "Dot/Renderer/Shader/Shader.h"
#include "Dot/Renderer/Camera/Camera.h"
#include "Dot/Renderer/Light/Light.h"
#include "Dot/Renderer/MaterialStack.h"
#include "Dot/Renderer/Buffers/FrameBuffer.h"

namespace Dot {

	enum RenderMode
	{
		NONE		= 0,
		OPAQUE		= BIT(1),
		TRANSLUCENT = BIT(2),
		ADDITIVE    = BIT(3)
	};

	struct RenderBatch
	{
		std::vector<Entity> entities;
	};
	
	class RenderSystem : public System
	{
	private:
		struct hash_pair
		{
			template <class T1, class T2>
			size_t operator()(const std::pair<T1, T2>& p) const
			{
				auto hash1 = std::hash<T1>{}(p.first);
				auto hash2 = std::hash<T2>{}(p.second);
				return hash1 ^ hash2;
			}
		};

		struct Compare
		{
			bool operator()(Entity a, Entity b)
			{
				return (a < b);
			}
		};
		Compare m_Cmp;
	public:
		RenderSystem();
		void BeginScene(const Camera& camera, const Ref<Light>& light);

		void Render();
		void AddFBO(Ref<Framebuffer> fbo);
		void RemoveFBO(Ref<Framebuffer> fbo);

		virtual void Add(Entity entity) override;
		virtual void Remove(Entity entity) override;

		void EndScene(const Ref<Shader>& shader);
	private:
		void setRenderStates(int32_t states);
		void removeFromContainer(std::vector<Entity>& container,Entity entity);
	private:	
		std::vector<Ref<Framebuffer>> m_FBOs;
		unsigned int m_FboIndex = 0;

		enum RenderMod
		{
			OPAQUE_MOD,
			TRANSPARENT_MOD,
			NUM_RENDER_MOD
		};
		struct Batch
		{
			std::unordered_map<MaterialID, std::vector<Entity> > material;
		};

		Batch m_Batch;
		
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
			glm::mat4 ViewMatrix;
			glm::mat4 ProjectionMatrix;
			glm::vec3 ViewPos;
		};

		struct LightData
		{
			glm::vec4 LightPosition;
			glm::vec4 LightColor;
			float	  LightStrength;
		};

		LightData m_LightData;
		SceneData m_SceneData;
		
	};
}