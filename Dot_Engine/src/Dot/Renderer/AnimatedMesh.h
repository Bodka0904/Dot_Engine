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

	class AnimatedMesh : public Renderable
	{
	public:
		AnimatedMesh(const std::string& Filename);
		~AnimatedMesh();

		virtual void Render(const Ref<Shader>& shader,int drawMod) override;

		void AnimateBones(float TimeInSeconds);
		void SetToDefaultPosition();

		unsigned int GetNumBones() const { return m_NumBones; }
	private:
		glm::vec3 calcInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
		glm::quat calcInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
		glm::vec3 calcInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);

		unsigned int findScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
		unsigned int findRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
		unsigned int findPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
		const aiNodeAnim* findNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);

		void loadHierarchy(const aiNode* pNode, std::vector<std::string>& boneMappingDelete);
		void readNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);

		void initMesh(unsigned int MeshIndex,const aiMesh* paiMesh,std::vector<AnimatedVertex>& vertices,std::vector<unsigned int>& Indices);
		void loadBones(unsigned int MeshIndex, const aiMesh* paiMesh, std::vector<VertexBoneData>& Bones);
	private:
		Ref<ArrayBuffer> m_VAO;

		std::vector<BoneInfo> m_BoneInfo;
		std::vector<SubMesh> m_SubMesh;


		std::map<std::string, unsigned int> m_BoneMapping; // maps a bone name to its index
		std::map<std::string, const aiNodeAnim*> m_NodeAnim;

		unsigned int m_NumBones;
		glm::mat4 m_InverseTransform;

		const aiScene* m_pScene;
		Assimp::Importer m_Importer;

		float m_Time = 0.0f;
		bool m_IsAnimated = false;
	};



}
