#pragma once

#include "Renderable.h""

#include <assimp/Importer.hpp>      
#include <assimp/scene.h>       
#include <assimp/postprocess.h>

#include <map>

namespace Dot {
	struct VertexBoneData
	{
#define NUM_BONES_PER_VERTEX 4
		VertexBoneData()
		{
			Reset();
		};

		void Reset()
		{
			memset(IDs, 0, sizeof(IDs));
			memset(Weights, 0, sizeof(Weights));
		}

		void AddBoneData(unsigned int BoneID, float Weight);

		unsigned int IDs[NUM_BONES_PER_VERTEX];
		float Weights[NUM_BONES_PER_VERTEX];
	};

	struct AnimatedVertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texcoord;
		VertexBoneData boneData;
	};

	struct BoneInfo
	{
		glm::mat4 BoneOffset;
		glm::mat4 FinalTransformation;
	};

	struct SubMesh
	{
		SubMesh()
		{
			NumIndices = 0;
			BaseVertex = 0;
			BaseIndex = 0;
		}
		unsigned int NumIndices;
		unsigned int BaseVertex;
		unsigned int BaseIndex;

	};

	struct Animation
	{
		Animation(const std::string& filename);
	
		const aiScene* pScene;
		std::map<std::string, const aiNodeAnim*> nodeAnim;
		std::map<std::string, unsigned int> boneMapping; 
		std::vector<BoneInfo> boneInfo;
		

		glm::mat4 inverseTransform;
		unsigned int numBones;
		float time = 0.0f;
	private:
		const aiNodeAnim* findNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);
		void loadHierarchy(const aiNode* pNode, std::vector<std::string>& boneMappingDelete);

		Assimp::Importer importer;
	};

	class AnimationComponent
	{
	public:
		std::map<std::string,Ref<Animation>> animation;
		bool isAnimating;
	};

	class SkinnedMesh : public Renderable
	{
	public:
		SkinnedMesh(const std::string& filename);
		void SetAnimation(Ref<Animation> animation);

		virtual void Render(const Ref<Shader>& shader, int drawMod)override;

		Ref<ArrayBuffer> vao;
		Ref<Material> material;
		std::vector<SubMesh> subMesh;
		std::vector<AnimatedVertex> vertices;
	private:
		void initMesh(unsigned int MeshIndex, const aiMesh* paiMesh, std::vector<AnimatedVertex>& vertices, std::vector<unsigned int>& Indices);
		void initAnimation(unsigned int MeshIndex, const aiMesh* paiMesh);
		Animation *skeleton;
		Assimp::Importer importer;
	};

}