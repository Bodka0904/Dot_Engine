#pragma once
#include "Dot/Renderer/Texture.h"
#include "Dot/Renderer/AnimatedMesh.h"
#include "Dot/Renderer/Mesh.h"
#include "Dot/Renderer/Shaders/Shader.h"


namespace Dot {
	
	// Very simple Asset Manager
	class AssetManager
	{
	public:
		AssetManager();
		void LoadAssets(const std::string& file);

		void LoadTexture(const std::string& name);
		void LoadShader(const std::string& name);
		void LoadAnimatedMesh(const std::string& name);
		void LoadInstancedMesh(const std::string& name);
		void LoadMesh(const std::string& name);
		void LoadCubeMap(const std::string& name);

		void UnLoadTexture(const std::string& name);
		void UnLoadShader(const std::string& name);
		void UnLoadAnimatedMesh(const std::string& name);
		void UnLoadInstancedMesh(const std::string& name);
		void UnLoadMesh(const std::string& name);
		void UnLoadCubeMap(const std::string& name);

		Ref<Texture2D>		GetTexture(const std::string& asset);
		Ref<CubeMapTexture> GetCubeMap(const std::string& asset);
		Ref<AnimatedMesh>	GetAnimMesh(const std::string& asset);
		Ref<Mesh>			GetMesh(const std::string& asset);
		Ref<InstancedMesh>  GetInstancedMesh(const std::string& asset);
		Ref<Shader>			GetShader(const std::string& asset);

		static Scope<AssetManager>& Get() { return s_Instance; }	
	private:
		std::vector<std::string> TokenizeString(const std::string& str, const std::string& delimiter);

	private:
		struct TextureAsset
		{
			Ref<Texture2D> asset;
			std::string name;
			std::string path;
			bool filters;
			bool flipped;
		};
		struct ShaderAsset
		{
			Ref<Shader> asset;
			std::string name;
			std::string path;
		};
		struct CubeMapAsset
		{
			Ref<CubeMapTexture> asset;
			std::string name;
			std::vector<std::string>face;
		};
		struct AnimatedMeshAsset
		{
			Ref<AnimatedMesh> asset;
			std::string name;
			std::string path;
		};
		struct InstancedMeshAsset
		{
			Ref<InstancedMesh> asset;
			std::string name;
			std::string path;
			unsigned int capacity;
		};
		struct MeshAsset
		{
			Ref<Mesh> asset;
			std::string name;
			std::string path;
		};

	private:	
		std::unordered_map <std::string, TextureAsset>		 m_Texture;
		std::unordered_map <std::string, CubeMapAsset>		 m_CubeMap;
		std::unordered_map <std::string, AnimatedMeshAsset>  m_AnimatedMesh;
		std::unordered_map <std::string, InstancedMeshAsset> m_InstancedMesh;
		std::unordered_map <std::string, MeshAsset>			 m_Mesh;
		std::unordered_map <std::string, ShaderAsset>		 m_Shader;

		static Scope<AssetManager> s_Instance;
	};
}